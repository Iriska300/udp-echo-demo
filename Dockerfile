FROM alpine:latest

ENV PORT=8080

# Устанавливаем netcat (nc) для HEALTHCHECK
RUN apk add --no-cache netcat-openbsd

COPY echo-server.sh /echo-server.sh
RUN chmod +x /echo-server.sh

EXPOSE 8080/udp

HEALTHCHECK --interval=10s --timeout=3s --start-period=5s --retries=3 \
  CMD nc -z -u 127.0.0.1 $PORT

CMD ["/echo-server.sh"]
