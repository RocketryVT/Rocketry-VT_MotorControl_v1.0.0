function dataconvert()
% DATACONVERT
% 
% Converts data logged from the micro SD card by the Arduino Motor
% Controller to a text based comma delimeted file (.csv). See the ICD for a
% list of available data types.
% 
% Note that even though the Arduino saves a file with the ".txt" extension,
% the data is not saved as text by the Arduino.
% 
% This function assumes that the Arduino will not change 
% 
% @author: Matt Marti
% @date: 2018-11-10

clear, clc

numskips = 0;
numtests = 0;
numunrecognized = 0;
outfile_extension = '.csv';

% File name
[file, path] = uigetfile('.TXT', 'Select the data file to convert', ...
    'Multiselect', 'on');
fnamein = [path, file];

% Open data file
fidin = fopen(fnamein, 'r');
fseek(fidin, 0, 'eof');
numbytes = ftell(fidin);
frewind(fidin);

% Open first data file
fidout = -1;
fidout_isopen = 0;
didwriteheader = 0;

% Go until detect file
progressbar = waitbar(0, 'Progress');
np = 0;
while ~feof(fidin)
    
    % Search for header
    pos = ftell(fidin);
    byte1 = fread(fidin, 1, 'uint8');
    byte2 = fread(fidin, 1, 'uint8');
    if isempty(byte2)
        break;
    end
    if (byte1 ~= 170 || byte2 ~= 20)
        if ~fidout_isopen
            fseek(fidin, -1, 'cof');
            continue;
        end
    end
    
    plen = fread(fidin, 1, 'uint8');
    ptype = fread(fidin, 1, 'uint8');
    try
        str = [byte1 byte2, plen, ptype, fread(fidin, plen-4, 'uint8')'];
    catch
        numunrecognized = numunrecognized + 1;
        continue;
    end
            
    % Start a new save file
    switch ptype
        case 1 % New Test flag
            if fidout_isopen
                fclose(fidout);
            end
            numtests = numtests + 1;
            fidout = fopen([fnamein(1:end-4) '-conv-' num2str(numtests) ...
                outfile_extension], 'w');
            fidout_isopen = 1;
            didwriteheader = 0;
        case 2 % End Test flag
            if fidout_isopen
                fclose(fidout);
                fidout_isopen = 0;
            end
        case 64 % 0x40
            if ~fidout_isopen
                continue;
            elseif ~didwriteheader
                fprintf(fidout, ['Time,Mode,Status,Temperature,',...
                    'LoadCell,NewData,ChecksumMismatch\n']);
                didwriteheader = 1;
            end
            
            % time
            a = bitshift(str(5), 24);
            b = bitshift(str(6), 16);
            c = bitshift(str(7), 8);
            d = bitshift(str(8), 0);
            time = bitor(bitor(bitor(a,b),c),d);
            fprintf(fidout, '%.6f', time*1e-6);
            
            % mode
            runmode = str(9);
            fprintf(fidout, ',%d', runmode);
            
            % error status
            a = cast(str(10),'uint16');
            b = cast(str(11),'uint16');
            status = bitor(bitshift(a,8),b);
            fprintf(fidout, ',%d', status);
            
            % temperature_combustion
            a = str(12);
            b = str(13);
            c = str(14);
            d = str(15);
            temperature = bytes_to_float(a,b,c,d);
            fprintf(fidout, ',%f', temperature);
            
            % thrust
            a = str(16);
            b = str(17);
            c = str(18);
            d = str(19);
            loadcell = bytes_to_float(a,b,c,d);
            fprintf(fidout, ',%f', loadcell);
            
            % new data flag
            newdata = str(20);
            fprintf(fidout, ',%d', newdata);
            str(20) = newdata;
            
            % Checksum
            c1 = str(21);
            c2 = str(22);
            str(21:22) = [c1, c2];
            checktest = xorchecksum(str);
            numskips = numskips + checktest;
            fprintf(fidout, ',%d\n', checktest);
            
            
        case 81 % 0x51
        case 82 % 0x52
        otherwise
            numunrecognized = numunrecognized + 1;
    end
    
    % Iterate
    waitbar(pos/numbytes);
    np = np + 1;
end

% Close things
fclose(fidin);
if fidout_isopen
    fclose(fidout);
end
close(progressbar);

% Output
fprintf('tests detected:       %d\n', numtests);
fprintf('packets decoded:      %d\n', np);
fprintf('unrecognized packets: %d\n', numunrecognized);
fprintf('skips:                %d\n', numskips);

end

function [ x ] = bytes_to_float( a, b, c, d )
%BYTES_TO_FLOAT Converts four unsigned char values to the corrosponding 32
%bit floating point value

a = cast(a, 'uint32');
b = cast(b, 'uint32');
c = cast(c, 'uint32');
d = cast(d, 'uint32');

aa = bitshift(a, 24);
bb = bitshift(b, 16);
cc = bitshift(c, 8);
dd = d;
x = bitor(bitor(bitor(aa, bb), cc), dd);

x = typecast(x,'single');


end

function failed = xorchecksum(str)
% Computes checksum, I know that the last two bytes are the checksum

c1 = 0;
c2 = 0;

len = length(str) - 2;
i = 1;
while i - 1 < len - 1
    c1 = bitxor(cast(c1, 'uint8'), cast(str(i), 'uint8'));
    c2 = bitxor(cast(c2, 'uint8'), cast(str(i+1), 'uint8'));
    i = i + 2;
end
if i - 1 < len
    c1 = bitxor(cast(c1, 'uint8'), cast(str(i), 'uint8'));
end

failed = c1 ~= str(end-1) || c2 ~= str(end);

end