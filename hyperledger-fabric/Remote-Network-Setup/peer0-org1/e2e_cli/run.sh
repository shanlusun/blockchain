
channel=mychannel

CHANNEL_NAME=$channel TIMEOUT=10000 docker-compose -f docker-compose-couch.yaml down

DOCKER_IMAGE_IDS=$(docker images | grep "dev\|none\|test-vp\|peer[0-9]-" | awk '{print $3}')
docker rmi -f $DOCKER_IMAGE_IDS

#./generateArtifacts.sh $channel

CHANNEL_NAME=$channel TIMEOUT=10000 docker-compose -f docker-compose-couch.yaml up -d 2>&1
