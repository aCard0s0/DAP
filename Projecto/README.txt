Para o sistema funcionar com todos os seus componentes é necessário:
  
1) Instalar todas as dependências necessárias
  

2) Inicializar o site e serviços REST no servidor virtual 

	a) java -jar drone.jar --static.path=public/”. O --static.path é especialmente importante, 
	pois define uma pasta de recursos estáticos fora do java archive, que podem ir sendo 
	adicionados, como as imagens dos parques.  

3) Inicializar no node controller os serviços 

	-Para correr o algoritmo de imagem: 
	a) Entrar na diretoria do projeto $ cd ../ProjectAlgorithm  
	b) Criar uma diretoria vazia “/build” e outra “/bin” dentro da diretoria do projeto. 
	c) Entrar na diretoria build: $ cd build  
	d) Compilar o projeto utilizando os comandos: $ cmake ..  e  $ make 
	e) Entrar na pasta /bin onde os executáveis foram criados: $ cd ../bin 
	f) E por fim correr o executável roads: $ ./road

	-Correr o ficheiro python: $ python3 startDrone.py 