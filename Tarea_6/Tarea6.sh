#!/bin/bash
#script para la instalacion de docker y docker-compose

#directorio general
cd ~

#eliminamos una instalacion anterior de docker
sudo apt-get remove docker docker-engine docker.io
#actualizamos repositorios
sudo apt-get update
sudo apt-get upgrade
#instalacion de dependecians de Docker
sudo apt-get install \
apt-transport-https \
ca-certificates \
curl \
software-properties-common
#Importamos la clave GCP
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
#verificar que la huella digital sea 9DC8 5822 9FC7 DD38 854A E2D8 8D81 803C 0EBF CD88 BUSCAMOS CON
sudo apt-key fingerprint 0EBFCD88
#añadimos el repositorios

sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
#actualizamos
sudo apt-get update
#instalamos docker
sudo apt-get install docker-ce
#probamos que todo este funcionando
sudo docker run hello-world

#añadimos el grupo de docker
sudo usermod -aG docker $USER

#instalando docker compose
# descargamos el binario
sudo curl -L "https://github.com/docker/compose/releases/download/1.23.1/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
#aplicamos permisos al binario
sudo chmod +x /usr/local/bin/docker-compose

#verificamos la instalacion de ambos
docker --version

docker-compose --version
