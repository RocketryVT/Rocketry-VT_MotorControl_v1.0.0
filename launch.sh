#!/bin/bash

tmux new -s control -d
tmux send-keys -t control './bin/main' C-m
tmux split-window -v -t control
tmux send-keys -t control 'watch -n 0.1 ./bin/bin2txt out.bin 20' C-m
tmux select-pane -t 0
tmux split-window -h -t control
tmux attach -t control
