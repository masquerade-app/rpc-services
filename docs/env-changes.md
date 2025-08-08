# Making Changes to the Development Environment

```bash
docker buildx build \
--push \
--platform linux/arm64,linux/amd64 --tag whuffman36/msqrd-rpc-env:latest .devcontainer
```