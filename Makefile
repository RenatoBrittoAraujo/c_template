IP = 
USER = 
PASS = 
PORT = 
SERVER_PROJECT_PATH = /home/$(USER)/app
PATH_MY_OS = 

exec_app:
	make clean
	make shared_prod
	ID_ANDAR=1 make all_dep_prod & \
	ID_ANDAR=2 make all_dep_prod

exec_main:
	make all_main_prod

# SERVIDOR MAIN
main_dev:
	make -f Makefile -C main/ dev

main_prod:
	make -f Makefile -C main/ prod

run_main_dev:
	make -f Makefile -C main/ run_dev

run_main_prod:
	make -f Makefile -C main/ run_prod

all_main_dev:
	make -f Makefile -C main/ all_dev

all_main_prod:
	make -f Makefile -C main/ all_prod

# SERVIDOR DEP
dep_dev:
	make -f Makefile -C dep/ dev

dep_prod:
	make -f Makefile -C dep/ prod

run_dep_dev:
	make -f Makefile -C dep/ run_dev

run_dep_prod:
	make -f Makefile -C dep/ run_prod

all_dep_dev:
	make -f Makefile -C dep/ all_dev

all_dep_prod:
	make -f Makefile -C dep/ all_prod

# SHARED
shared_dev:
	make -f Makefile -C shared/ clean
	make -f Makefile -C shared/ dev

shared_prod:
	make -f Makefile -C shared/ clean
	make -f Makefile -C shared/ prod

# TODOS OS SERVIDORES
all_system_dev:
	make dep_dev
	make main_dev

	make run_main_dev &\
	make run_dep_dev &\
	make run_dep_dev	

all_system_prod:
	make dep_prod
	make main_prod

	make run_main_prod &\
	make run_dep_prod &\
	make run_dep_prod

#  CONFIGURACOES DE DEPLOY
deps_dev:
	sudo apt install docker-compose
	sudo apt install sshpass

ssh:
	sshpass -p $(PASS) ssh $(USER)@$(IP) -p $(PORT) 

install:
	rsync -av --exclude=".git" -e 'sshpass -p $(PASS) ssh -p $(PORT)' . $(USER)@$(IP):$(SERVER_PROJECT_PATH)
	@echo "app installed on target:$(SERVER_PROJECT_PATH)"

clean:
	make -C shared clean
	make -C dep clean
	make -C main clean
	@echo " find . -type f -name '*.o' -delete"; find . -type f -name '*.o' -delete

commita:
	@git add .
	@git commit -m "$(shell date)"
	@git push origin master

.PHONY: clean