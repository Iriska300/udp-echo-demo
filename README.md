# UDP Echo Demo for K3s

UDP-эхо-сервер и клиент на C++ для развертывания в K3s. Клиент отправляет "ping", сервер возвращает "ping" — режим "пинг-понг".

## Структура проекта

```
udp-echo-demo/
├── client/          # C++ UDP-клиент
├── server/          # C++ UDP-эхо-сервер
├── k8s/             # Манифесты для K3s
└── docker-compose.yml
```

## Быстрый старт

```bash
# 1. Клонируйте репозиторий
git clone https://github.com/Iriska300/udp-echo-demo.git
cd udp-echo-demo

# 2. Соберите Docker-образы
docker build -t udp-echo-server:latest ./server
docker build -t udp-echo-client:latest ./client

# 3. Загрузите образы в K3s
docker save udp-echo-server:latest | sudo k3s ctr images import -
docker save udp-echo-client:latest | sudo k3s ctr images import -

# 4. Разверните в кластере
kubectl apply -f k8s/

# 5. Проверьте логи (пинг-понг!)
kubectl logs -f -l app=udp-echo-client
```

## Проверка работы

```bash
# Статус подов
kubectl get pods

# Логи сервера
kubectl logs -l app=udp-echo-server

# Проверка DNS внутри кластера
kubectl run -it --rm dns-test --image=busybox -- nslookup udp-echo-service

# Проверка UDP доступности
kubectl run -it --rm udp-test --image=busybox -- sh -c "echo 'test' | nc -u -w1 udp-echo-service 8080"
```

## Ожидаемый вывод

В логах клиента вы увидите:

```
Resolving udp-echo-service...
Server IP: 10.43.xxx.xxx
Sending: ping #1
Received: ping #1
Sending: ping #2
Received: ping #2
...
```

## Удаление

```bash
# Удалить все ресурсы из кластера
kubectl delete -f k8s/

# Удалить образы из K3s (опционально)
sudo k3s ctr images rm udp-echo-server:latest
sudo k3s ctr images rm udp-echo-client:latest
```

##  Технологии

- **Язык:** C++
- **Контейнеризация:** Docker
- **Оркестрация:** K3s
- **Сеть:** UDP, Kubernetes Services, CoreDNS
