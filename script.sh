#!/bin/bash

port=8080

pid=$(lsof -t -i :$port)

# Check if a process is using the specified port
if [[ -n $pid ]]; then
    echo "Process with PID $pid is using port $port."
    echo "Killing the process..."
    
    kill $pid
    
    sleep 1
    
    if ps -p $pid > /dev/null; then
        echo "Failed to kill the process with PID $pid."
    else
        echo "Process with PID $pid has been terminated."
    fi
else
    echo "No process is using port $port."
fi
