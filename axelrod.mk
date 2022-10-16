a.out: axelrod.o mc_server_2.o pd_server.o pbPlots.o supportLib.o
	gcc axelrod.o mc_server_2.o pd_server.o pbPlots.o supportLib.o
axelrod.o: axelrod.c mc_server.h pd_server.h pbPlots.h supportLib.h
	gcc -c axelrod.c
mc_server_2.o: mc_server_2.c mc_server.h pd_server.h
	gcc -c mc_server_2.c
pd_server.o: pd_server.c pd_server.h
	gcc -c pd_server.c
pbPlots.o: pbPlots.c pbPlots.h
	gcc -c pbPlots.c
supportLib.o: supportLib.c supportLib.h
	gcc -c supportLib.c
