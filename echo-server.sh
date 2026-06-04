#!/bin/sh
echo "UDP Echo Server started on port 8080"

# Создаём именованный канал для передачи данных
mkfifo /tmp/fifo

# Запускаем netcat в цикле
while true; do
  nc -u -l -p 8080 < /tmp/fifo | tee /tmp/fifo
  sleep 1
done
