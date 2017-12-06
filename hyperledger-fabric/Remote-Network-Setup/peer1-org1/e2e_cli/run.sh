
docker-compose -f one_peer_docker-compose-e2e.yaml down

DOCKER_IMAGE_IDS=$(docker images | grep "dev\|none\|test-vp\|peer[0-9]-" | awk '{print $3}')
docker rmi -f $DOCKER_IMAGE_IDS

docker-compose -f one_peer_docker-compose-e2e.yaml up -d 2>&1
