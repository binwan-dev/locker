TAG=`head -1 releaselog`
RegistryServer="registry.fnlinker.com"
RegistryRepo="fnlinker-public/locker_server"
RegistryUser="nfadmin"
RegistryPassword="1qa@WS\#ed\$$rf"

create-tag:
	echo `date "+%y%m%d%H%M%S"` > releaselog

docker-publish: create-tag
	- docker build -t ${RegistryServer}/${RegistryRepo}:${TAG} .
	- docker login ${RegistryServer} -u ${RegistryUser} -p ${RegistryPassword}
	- docker push ${RegistryServer}/${RegistryRepo}:${TAG}
	- rm -rf releaselog
