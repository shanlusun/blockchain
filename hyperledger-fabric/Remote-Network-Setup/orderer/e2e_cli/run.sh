#for solo
docker-compose -f solo_one_orderer_docker-compose-e2e.yaml down

docker-compose -f solo_one_orderer_docker-compose-e2e.yaml up -d 2>&1

#for kafka
#docker-compose -f kafka_one_orderer_docker-compose-e2e.yaml down

#DOCKER_IMAGE_IDS=$(docker images | grep "dev\|none\|test-vp\|peer[0-9]-" | awk '{print $3}')
#docker rmi -f $DOCKER_IMAGE_IDS
#rm -rf ./kafka1
#docker-compose -f kafka_one_orderer_docker-compose-e2e.yaml up -d 2>&1
