#!/bin/bash

tmux new -s control -d
tmux send-keys -t control './bin/main' C-m
tmux split-window -v -t control
sleep 0.5
tmux send-keys -t control './bin/readin out.bin' C-m
tmux select-pane -t 0
tmux split-window -h -t control
tmux send-keys -t control './bin/pack' C-m
tmux attach -t control
