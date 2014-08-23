%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:-use_module(library(sockets)).
:-use_module(library(random)).

port(60070).
:-dynamic s/1.



% launch me in sockets mode
server:-
        port(Port),
        socket_server_open(Port, Socket),
        socket_server_accept(Socket, _Client, Stream, [type(text)]),
        write('Accepted connection'), nl,
		retractall(s(_)),
		assert(s(Socket)),
        serverLoop(Stream),
        socket_server_close(Socket).

serverClose:- s(S),
socket_server_close(S).
		
% wait for commands
serverLoop(Stream) :-
        repeat,
        read(Stream, ClientMsg),
        write('Received: '), write(ClientMsg), nl,
        parse_input(ClientMsg, MyReply),
        format(Stream, '~q.~n', [MyReply]),
        write('Wrote: '), write(MyReply), nl,
        flush_output(Stream),
        (ClientMsg == quit; ClientMsg == end_of_file), !.

		
%Funçoes de Jogo

parse_input(insere_pecaY(Y,X,Jogador,Tab), Answer) :-
        (insere_pecaY(Y,X,Jogador,Tab,Answer)->true;Answer='No'),!.
		
parse_input(rodaY(NumLinha,Tab,Direc), Answer) :-
        rodaY(NumLinha,Tab,Answer,Direc),!.
		
parse_input(rodaX(Coluna,Tab,Ult,Baixo), Answer) :-
        rodaX(Coluna,Tab,Answer,Ult,Baixo),!.

parse_input(calcula_jogada(Tab,Nivel,Jogador), Answer) :-
       %(calcula_jogada(Tab,Tipo,X,Y,Nivel,Jogador)->Answer=Tipo-X-Y;write('Deu falso'),nl,parse_input(calcula_jogada(Tab,Tipo,X,Y,Nivel,Jogador), Answer=tipo-X-Y)),!.
		calcula_jogada(Tab,Tipo,X,Y,Nivel,Jogador),Answer=Tipo-X-Y,!.

parse_input(fim_jogo(Tab), Answer) :-
        fim_jogo(Tab,Answer),!.		


parse_input(quit, ok-bye) :- !.
                
comando(Arg1, Arg2, Answer) :-
        write(Arg1), nl, write(Arg2), nl,
        Answer = 5.
tabuleiro( [[0, 0, 0, 0, 0, 0, 0],[2, 1, 0, 0, 0, 0, 0],[1, 1, 1, 2, 0, 0, 0],[0, 0, 2, 1, 0, 0, 0],[0, 0, 2, 2, 1, 0, 0],[0, 0, 0, 0, 0, 0, 0],[0, 0, 0, 0, 0, 0, 0]]).
printme(X,Answer):-
            write('HELLO WORLD!'),
            tabuleiro(Y),
            Answer=Y.
     
%-------------------
% DESENHA TABULEIRO
%-------------------

% visualiza_estado(+Tabuleiro)
visualiza_estado(Tab) :- write(' '),
                         desenha_tab_numero(7), nl,
                         write('  +---+---+---+---+---+---+---+'), nl,
                         desenha_tab(0,Tab),
                         write(' '),
                         desenha_tab_numero(7), nl.

% desenha_tab(+NumLinha,+Tab)
desenha_tab(N,[X|Y]) :- N < 7,
                        N1 is N + 1,
                        write(N1),
                        write(' | '),
                        desenha_tab_linha(N1,X),
                        desenha_tab(N1,Y).
desenha_tab(_,[]).

% desenha_tab_linha(+NumLinha,+Linha)
desenha_tab_linha(N1,[X|Y]) :- X \= [],
                               X = 0,
                               write(' '),
                               write(' | '),
                               desenha_tab_linha(N1,Y).
desenha_tab_linha(N1,[X|Y]) :- X \= [],
                               X \= 0,
                               write(X),
                               write(' | '),
                               desenha_tab_linha(N1,Y).
desenha_tab_linha(N1,[_|Y]) :- Y = [],
                               write(N1),
                               desenha_tab_linha(Y).
desenha_tab_linha([]) :- nl, write('  +---+---+---+---+---+---+---+'), nl.

% desenha_tab_numero(+NumColuna)
desenha_tab_numero(N) :- desenha_tab_numero(N,0).
desenha_tab_numero(N,N).
desenha_tab_numero(N,C) :- N > C,
                           C1 is C+1,
                           write('   '),
                           write(C1),
                           desenha_tab_numero(N,C1).	 
            
            
%--------------
% INSERIR PEÇA
%--------------

% insere_pecaY(+Y,+X,+Jogador,+Tab,-NovoTab)
insere_pecaY(-1,_,_,Inicial,Final) :- Final = Inicial.
insere_pecaY(1,Pos,Elem,[H|T],[H2|T2]) :- insere_pecaX(H,1,Pos,Elem,H2),
                                          insere_pecaY(-1, Pos, Elem, T, T2).
insere_pecaY(Y,Pos,Elem,[H|T],[H|T2]) :- Y > 1,
                                         Y1 is Y - 1,
                                         insere_pecaY(Y1,Pos,Elem,T,T2).

% insere_pecaX(+Tab,+Y,+X,+Jogador,-NovoTab)
insere_pecaX([],_,_,_,[]).
insere_pecaX([H|T],Pos,Pos,Elem,[H2|T2]) :- !,
                                            H = 0,
                                            H2 = Elem,
                                            Pos1 is Pos + 1,
                                            insere_pecaX(T,Pos1,Pos,Elem,T2).
insere_pecaX([H|T],Pos1,Pos,Elem,[H|T2]) :- Pos2 is Pos1 + 1,
                       insere_pecaX(T,Pos2,Pos,Elem,T2).
                                            

%-----------------------------------------------
% Rodar Linha  Direc: 1 - direita, 2 - esquerda
%-----------------------------------------------
%-------- 1 - Direita

% rodaY(+NumLinha,+Tab,-NovoTab,+Direc)
rodaY(-1,Inicial,Final,_) :- Final = Inicial.
rodaY(1,[H|T],[H2|T2],Direc) :- rodaY2(H,H2,Direc),
                                rodaY(-1,T,T2,Direc).
rodaY(Y,[H|T],[H|T2],Direc) :- Y > 1,
                               Y1 is Y - 1,
                               rodaY(Y1,T,T2,Direc).

% rodaY2(+Linha,-Linha,+Direita)
rodaY2([],[],2).
rodaY2([_|T],[H2|T2],2) :- setH2(T,H2,2),
                           rodaY2(T,T2,2).

setH2([],0,2).
setH2([X|_],X,2).

%-------- 2 - Esquerda

% rodaY2(+Linha,-Linha,+Esquerda)
rodaY2(0,[],1).
rodaY2([_|T],[_|T2],1) :- T = [],
                          T2 = [].
rodaY2([H|T],[H2|T2],1) :- H2 = 0,
                           setH2(H,T2,1),
                           rodaY2(T,T2,1).
rodaY2([H|T],[_|T2],1) :- setH2(H,T2,1),
                          rodaY2(T,T2,1).

setH2([],0,1).
setH2(X,[X|_],1).


%------------------------------------------
% Rodar Coluna  Direc: 1 - baixo, 2 - cima
%------------------------------------------
%-------- 1 - Baixo
% rodaX(+Coluna,+Tab,-NovoTab,+Ult,+Baixo)
rodaX(Pos,[H|[]],[H2|[]],Ult,_) :- rodaX2(Pos,1,H,H2,Ult,_,1).
rodaX(Pos,[H|T],[H2|T2],Ult,1) :- rodaX2(Pos,1,H,H2,Ult,NovoUlt,1),
                                  rodaX(Pos,T,T2,NovoUlt,1).

rodaX2(_,-1,Inicial,Final,_,_,1) :- Final = Inicial.
rodaX2(Pos,Pos,[H|T],[H2|T2],Ult,NovoUlt,1) :- H2 = Ult,
                                               NovoUlt = H,
                                               rodaX2(Pos,-1,T,T2,NovoUlt,_,1).
rodaX2(Pos,Pos,[H|T],[_|T2],_,NovoUlt,1) :- NovoUlt = H,
                                            rodaX2(Pos,-1,T,T2,NovoUlt,_,1).
rodaX2(Pos,Inic,[H|T],[H|T2],Ult,NovoUlt,1) :- Inic1 is Inic + 1,
                                               rodaX2(Pos,Inic1,T,T2,Ult,NovoUlt,1).

%-------- 2 - Cima
% rodaX(+Coluna,+Tab,-NovoTab,+Ult,+Cima)
rodaX(Pos,[H|[HT|T]],[H2|T2],Ult,2) :- rodaX2(Pos,1,H,HT,H2,0,2),
                                       rodaX(Pos,[HT|T],T2,Ult,2).
rodaX(Pos,[H|[]],[H2|_],_,2) :- rodaX2(Pos,1,H,_,H2,1,2).

rodaX2(_,-1,Inicial,_,Final,_,2) :- Final = Inicial.
rodaX2(Pos,Pos,[_|T],[H2|T2],[H3|T3],0,2) :- H3 = H2,
                                             rodaX2(Pos,-1,T,T2,T3,0,2).
rodaX2(Pos,Pos,[_|T],[_|T2],[H3|T3],1,2) :- H3 = 0,
                                            rodaX2(Pos,-1,T,T2,T3,1,2).
rodaX2(Pos,Inic,[H|T],[_|T2],[H|T3],Ult,2) :- Inic1 is Inic + 1,
                                              rodaX2(Pos,Inic1,T,T2,T3,Ult,2).



%-----------------------
% VERIFICAR FIM DE JOGO
%-----------------------

% fim_jogo(+Tab,-Jogador)
fim_jogo(Tab,Jogador) :- Jog1 is 1,
                         fim_jogoJ(Tab,Jog1,Win1),
                         Win1 = 1,
                         Jog2 is 2,
                         fim_jogoJ(Tab,Jog2,Win2),
                         Win2 = 1,
                         Jogador is 3,
                         write('     ** Empate **'),nl.
fim_jogo(Tab,Jogador) :- Jog is 1,
                         fim_jogoJ(Tab,Jog,Win),
                         Win = 1,
                         Jogador is 1,
                         write('     ** O Jogador 1 é o vencedor! **'),nl.
fim_jogo(Tab,Jogador) :- Jog is 2,
                         fim_jogoJ(Tab,Jog,Win),
                         Win = 1,
                         Jogador is 2,
                         write('     ** O Jogador 2 é o vencedor! **'),nl.
fim_jogo(_,-1).

% fim_jogo(+Tab,+Jogador,-Win)
% Win = 1 -> Ganhou
% Win /=1 -> Não Ganhou
%Isto é tipo um if. Se tiver nas linhas ja nao faz mais nd
fim_jogoJ(Tab,Jogador,Win) :- verTodasLinhas(Tab,Jogador,Result),
                              Result = 1,
                              Win = 1.
%Isto é tipo else if, se falhar a 1a ve esta
fim_jogoJ(Tab,Jogador,Win) :- verTodasColunas(Tab,Jogador,Result),
                              Result = 1,
                              Win = 1.
%outro else if
fim_jogoJ(Tab,Jogador,Win) :- verTodasDiagonais(Tab,Jogador,Result),
                              Result = 1,
                              Win = 1.
%else. Se as outras falharam todas entao ninguem ganhou
fim_jogoJ(_,_,0).



%-------------------------------------
% JOGADAS DO COMPUTADOR - NIVEL FÁCIL
%-------------------------------------

% calcula_jogada(+Tab,-NovoTab,+Nivel,+Jogador)
calcula_jogada(Tab,Tipo,X,Y,1,Jogador) :- write('GOT HERE'),nl,
										escolhe_tipo_jogada_aleatoria(TipoJogada,5),
										write('Tipo is:'),nl, write(TipoJogada),nl,
                                         jogada_aleatoria(TipoJogada,Tab,Tipo,X,Y,Jogador).

% existe maior possibilidade de escolher inserir peça (3/5)
% jogada_aleatoria(+TipoJogada,+Tab,-NovoTab,+Jogador)
jogada_aleatoria(2,Tab,Tipo,Y,Direc,_) :- escolhe_num_aleatorio(Y),
                                     escolhe_direc_aleatoria(Direc),
                                     write('Rodei a linha '), write(Y),
                                     write(' para a '), escreve_direcL(Direc),
                                     rodaY(Y,Tab,_,Direc),
									 Tipo=rodarY.
jogada_aleatoria(4,Tab,Tipo,X,Direc,_) :- escolhe_num_aleatorio(X),
                                     escolhe_direc_aleatoria(Direc),
                                     write('Rodei a coluna '), write(X),
                                     write(' para '), escreve_direcC(Direc),
                                     rodaX(X,Tab,NovoTab,0,Direc),
									 Tipo=rodarX.
jogada_aleatoria(_,Tab,Tipo,X,Y,Jogador) :- escolhe_num_aleatorio(X),
                                           escolhe_num_aleatorio(Y),
                                           insere_pecaY_a(Y,X,Jogador,Tab,NovoTab),
                                           write('Inseri uma peça, nas seguintes coordenadas:'), nl,
                                           write(' X = '), write(X), nl,
                                           write(' Y = '), write(Y),
										   Tipo=inserir.
%Se as coordenadas anteriores escolhidas falharem, então volta a chamar a funcao novamente
jogada_aleatoria(Z,Tab,Tipo,X,Y,Jogador) :- jogada_aleatoria(Z,Tab,Tipo,X,Y,Jogador).



%---------------------------------------
% JOGADAS DO COMPUTADOR - NIVEL MÉDIO
%---------------------------------------

% calcula_jogada(+Tab,-NovoTab,+Nivel,+Jogador)
% calcula_jogada(+Tab,-NovoTab,+Nivel,+Jogador)
calcula_jogada(Tab,Tipo,X,Y,2,Jogador) :- escolhe_tipo_jogada_aleatoria(TipoJogada,10),
										 get_jogada(Tab,Tipo,X,Y,2,Jogador,TipoJogada).
										 
										 
get_jogada(Tab,Tipo,X,Y,2,Jogador,3):- jogada_inteligente(3,Tab,Tipo,X,Y,Jogador).
										 
get_jogada(Tab,Tipo,X,Y,2,Jogador,7) :- jogada_inteligente(7,Tab,Tipo,X,Y,Jogador).
										 
get_jogada(Tab,Tipo,X,Y,2,Jogador,TipoJogada) :- jogada_inteligente(TipoJogada,Tab,NovoTab,Jogador),
										visualiza_estado(NovoTab),
										compareboard(Tab,NovoTab,0,0,X,Y),Tipo=inserir.

% existe maior possibilidade de escolher inserir peça (8/10)
% jogada_inteligente(+TipoJogada,+Tab,-NovoTab,+Jogador)
jogada_inteligente(3,Tab,Tipo,Y,Direc,_) :- escolhe_num_aleatorio(Y),
                                       escolhe_direc_aleatoria(Direc),
                                       write('Rodei a linha '), write(Y),
                                       write(' para a '), escreve_direcL(Direc),
                                       rodaY(Y,Tab,NovoTab,Direc),
									   Tipo=rodarY.
jogada_inteligente(7,Tab,Tipo,X,Direc,_) :- escolhe_num_aleatorio(X),
                                       escolhe_direc_aleatoria(Direc),
                                       write('Rodei a coluna '), write(X),
                                       write(' para '), escreve_direcC(Direc),
                                       rodaX(X,Tab,NovoTab,0,Direc),
									   Tipo=rodarX.
jogada_inteligente(_,Tab,NovoTab,Jogador) :- joga_dificil(Tab,NovoTab,Jogador,Conseguiu,4),
                                             ( Conseguiu = 0 -> escolhe_num_aleatorio(X),
                                               escolhe_num_aleatorio(Y),
                                               insere_pecaY_a(Y,X,Jogador,Tab,NovoTab),
                                               write('Inseri uma peça, nas seguintes coordenadas:'), nl,
                                               write(' X = '), write(X), nl,
                                               write(' Y = '), write(Y) ; true).
%Se as coordenadas anteriores escolhidas falharem, então volta a chamar a funcao novamente
jogada_inteligente(Z,Tab,NovoTab,Jogador) :- jogada_inteligente(Z,Tab,NovoTab,Jogador).


%---------------------------------------
% JOGADAS DO COMPUTADOR - NIVEL DIFICIL
%---------------------------------------

% calcula_jogada(+Tab,-NovoTab,+Nivel,+Jogador)
calcula_jogada(Tab,Tipo,X,Y,3,Jogador) :- escolhe_tipo_jogada_aleatoria(TipoJogada,10),
										 get_jogada(Tab,Tipo,X,Y,3,Jogador,TipoJogada).
										 
										 
get_jogada(Tab,Tipo,X,Y,3,Jogador,3):- jogada_inteligente(3,Tab,Tipo,X,Y,Jogador).
										 
get_jogada(Tab,Tipo,X,Y,3,Jogador,7) :- jogada_inteligente(7,Tab,Tipo,X,Y,Jogador).
										 
get_jogada(Tab,Tipo,X,Y,3,Jogador,TipoJogada) :- jogada_inteligente(TipoJogada,Tab,NovoTab,Jogador),
										visualiza_estado(NovoTab),
										compareboard(Tab,NovoTab,0,0,X,Y),Tipo=inserir.

% existe maior possibilidade de escolher inserir peça (8/10)
% jogada_inteligente(+TipoJogada,+Tab,-NovoTab,+Jogador)
jogada_inteligente(3,Tab,Tipo,Y,Direc,_) :- escolhe_num_aleatorio(Y),
                                       escolhe_direc_aleatoria(Direc),
                                       write('Rodei a linha '), write(Y),
                                       write(' para a '), escreve_direcL(Direc),
                                       rodaY(Y,Tab,NovoTab,Direc).
jogada_inteligente(7,Tab,Tipo,X,Direc,_) :- escolhe_num_aleatorio(X),
                                       escolhe_direc_aleatoria(Direc),
                                       write('Rodei a coluna '), write(X),
                                       write(' para '), escreve_direcC(Direc),
                                       rodaX(X,Tab,NovoTab,0,Direc).
jogada_inteligente(_,Tab,NovoTab,Jogador) :- joga_dificil(Tab,NovoTab,Jogador,Conseguiu,8),
                                             ( Conseguiu = 0 -> escolhe_num_aleatorio(X),
                                               escolhe_num_aleatorio(Y),
                                               insere_pecaY_a(Y,X,Jogador,Tab,NovoTab),
                                               write('Inseri uma peça, nas seguintes coordenadas:'), nl,
                                               write(' X = '), write(X), nl,
                                               write(' Y = '), write(Y) ; true).
%Se as coordenadas anteriores escolhidas falharem, então volta a chamar a funcao novamente
jogada_inteligente(Z,Tab,NovoTab,Jogador) :- jogada_inteligente(Z,Tab,NovoTab,Jogador).

board_position([[E|_]|_],X,Y,E):-X=0,Y=0.
board_position([[_|R]|R1],X,Y,Element):-Y=0,X1 is X-1,board_position([R|R1],X1,Y,Element).
board_position([[_|_]|R1],X,Y,Element):-Y1 is Y-1,board_position(R1,X,Y1,Element).

compareboard([[E|T]|R2],[[X|R]|R1],Xstart,Ystart,Xnew,Ynew):-X\=0,E==0,Xnew is Xstart, Ynew is Ystart.
compareboard([[E|T]|R2],[[X|R]|R1],Xstart,Ystart,Xnew,Ynew):-X\=0,E==X,Xn is Xstart+1,compareboard([T|R2],[R|R1],Xn,Ystart,Xnew,Ynew).
compareboard([[E|T]|R2],[[X|R]|R1],Xstart,Ystart,Xnew,Ynew):-X==0,Xn is Xstart+1,compareboard([T|R2],[R|R1],Xn,Ystart,Xnew,Ynew).
compareboard([[_|_]|R2],[[_|_]|R1],Xstart,Ystart,Xnew,Ynew):-Yn is Ystart+1,compareboard(R2,R1,0,Yn,Xnew,Ynew).
%--------------------------------
% PREDICADOS PARA OS DOIS NIVEIS
%--------------------------------

% escolher_tipo_jogada_aleatoria(-Resultado,+Limite_maximo)
escolhe_tipo_jogada_aleatoria(X,Y) :- random(0,Y,R),
                                      X is R + 1.

% escolher_num_aleatorio(-Resultado)
escolhe_num_aleatorio(Num) :- 
							random(0,7,R),
                              Num is R + 1.

% escolher_direc_aleatoria(-Direccao)
escolhe_direc_aleatoria(Direc):- random(0,2,R),
                                 Direc is R + 1.

% escreve_direcL(+DireccaoLinhas)
escreve_direcL(1) :- write('direita').
escreve_direcL(2) :- write('esquerda').

% escreve_direcC(+DireccaoColunas)
escreve_direcC(1) :- write('baixo').
escreve_direcC(2) :- write('cima').

%------------------------------------------------------
% INSERIR PEÇA ALEATORIA PARA UMA JOGADA DO COMPUTADOR
%------------------------------------------------------

% insere_pecaY_a(+Y,+X,+Jogador,+Tab,-NovoTab)
insere_pecaY_a(-1,_,_,Inicial,Final) :- Final = Inicial.
insere_pecaY_a(1,Pos,Elem,[H|T],[H2|T2]) :- insere_pecaX_a(H,1,Pos,Elem,H2),
                                            insere_pecaY_a(-1, Pos, Elem, T, T2).
insere_pecaY_a(Y,Pos,Elem,[H|T],[H|T2]) :- Y > 1,
                                           Y1 is Y - 1,
                                           insere_pecaY_a(Y1,Pos,Elem,T,T2).

% insere_pecaX_a(+Tab,+Y,+X,+Jogador,-NovoTab)
insere_pecaX_a([],_,_,_,[]).
insere_pecaX_a([H|T],Pos,Pos,Elem,[H2|T2]) :- !,
                                              H = 0,
                                              H2 = Elem,
                                              Pos1 is Pos + 1,
                                              insere_pecaX_a(T,Pos1,Pos,Elem,T2).
insere_pecaX_a([H|T],Pos1,Pos,Elem,[H|T2]) :- Pos2 is Pos1 + 1,
                                              insere_pecaX_a(T,Pos2,Pos,Elem,T2).

%------------------
% VERIFICAR LINHAS
%------------------
% 1 -> Ganhou
% 0 -> Não Ganhou

% verTodasLinhas(+Tab,+Jogador,-Win)
verTodasLinhas(Tab,Jogador,Win) :- verificaLinhas(Tab,Jogador,1,0,Win).
% verificaLinhas(+Tab,+Linha,+Jogador,+Encontrados,-Win)
verificaLinhas(_,_,_,1,Win) :- Win = 1.   % Se o jogador ganhou
verificaLinhas([],_,_,_,0).           % Se o jogador não ganhou
verificaLinhas([H|T],Jogador,Linha,_,Win) :- Linha1 is Linha + 1,
                                           verificaLinha(H,Jogador,0,0,Fin),
                                           verificaLinhas(T,Jogador,Linha1,Fin,Win).

% verificaLinha(+Tab,+Jogador,+Encontrados,+Ini,-Fin)
verificaLinha([],_,Encontrados,0,0) :- Encontrados < 4.
verificaLinha(_,_,Encontrados,0,1) :- Encontrados > 3.
verificaLinha([H|T],Jogador,Encontrados,Ini,Fin) :- H = Jogador,
                                                 Encontrados1 is Encontrados + 1,
                                                 verificaLinha(T,Jogador,Encontrados1,Ini,Fin).
verificaLinha([H|T],Jogador,_,Ini,Fin) :- H \= Jogador,
                                          verificaLinha(T,Jogador,0,Ini,Fin).


%-------------------
% VERIFICAR COLUNAS
%-------------------

% verTodasColunas(+Tab,+Jogador,-Win)
verTodasColunas(Tab,Jogador,Win) :- verificaColunas(Tab,Jogador,0,Win).

% verificaColunas(+Tab,+Jogador,+Linha,-Encontrou)
verificaColunas(_,_,7,Encontrou) :- Encontrou = 0.
verificaColunas(Tab,Jogador,Linha,Encontrou) :- Linha < 7,
                                                verificaColuna(Tab,Jogador,Linha,0,Enc),
                                                Enc = 1,
                                                Encontrou = 1.
verificaColunas(Tab,Jogador,Linha,Encontrou) :- Linha1 is Linha + 1,
                                                Linha < 7,
                                                verificaColuna(Tab,Jogador,Linha,0,Enc),
                                                Enc = 0,
                                                verificaColunas(Tab,Jogador,Linha1,Encontrou).

% verificaColuna(+Tab,+Jogador,+Linha,+Encontrados,-Encontrado)
verificaColuna([],_,_,Encontrados,0) :- Encontrados < 4.
verificaColuna(_,_,_,Encontrados,1) :- Encontrados > 3.
verificaColuna([H|T],Jogador,Obj,Encontrados,Quatro) :- verificaColuna2(H,Jogador,0,Obj,Encontrou),
                                                        Encontrou = 1,
                                                        Encontrados1 is Encontrados + 1,
                                                        verificaColuna(T,Jogador,Obj,Encontrados1,Quatro).
verificaColuna([H|T],Jogador,Obj,_,Quatro) :- verificaColuna2(H,Jogador,0,Obj,Encontrou),
                                              Encontrou = 0,
                                              Encontrados1 is 0,
                                              verificaColuna(T,Jogador,Obj,Encontrados1,Quatro).

% verificaColuna2(+Tab,+Jogador,+Current,+Obj,-Encontrou)
verificaColuna2([H|_],Jogador,Obj,Obj,Encontrou) :- H = Jogador,
                                                    Encontrou = 1.
verificaColuna2([H|_],Jogador,Obj,Obj,Encontrou) :- H \= Jogador,
                                                    Encontrou = 0.
verificaColuna2([_|T],Jogador,Current,Obj,Encontrou) :- Current \= Obj,
                                                        Current1 is Current + 1,
                                                        verificaColuna2(T,Jogador,Current1,Obj,Encontrou).


%-----------------------------------------------------
% INVERTER TABULEIRO PARA A VERIFICAÇÃO DAS DIAGONAIS
%-----------------------------------------------------

% inverteTab(+Tab,-NovoTab)
inverteTab([],[]).
inverteTab([H|T],[H2|T2]) :- inverte(H,H2),
                             inverteTab(T,T2).
inverte(Lista,InvLista) :- rev(Lista,[],InvLista).

rev([H|T],S,R) :- rev(T,[H|S],R).
rev([],R,R).


%---------------------
% VERIFICAR DIAGONAIS
%---------------------

% verTodasDiagonais(+Tab,+Jogador,-Result)
verTodasDiagonais(_,_,0).
verTodasDiagonais(Tab,Jogador,Result) :- verDiagonais(Tab,Jogador,Winer),
                                         Winer = 1,
                                         Result = 1.
verTodasDiagonais(Tab,Jogador,Result) :- inverteTab(Tab,Tab2),
                                         verDiagonais(Tab2,Jogador,Winer),
                                         Winer = 1,
                                         Result = 1.

% verDiagonais(+Tab,+Jogador,-Result)
verDiagonais(_,_,0).
verDiagonais(Tab,Jogador,Result) :- verDiagonaisDireita(Tab,Jogador,0,0,Winer),
                                    Winer = 1,
                                    Result = 1.
verDiagonais(Tab,Jogador,Result) :- verDiagonaisBaixo(Tab,Jogador,0,0,Winer2),
                                    Winer2 = 1,
                                    Result = 1.

% verDiagonaisBaixo(+Tab,+Jogador,+Objectivo,+Encontrados,-Quatro)
verDiagonaisBaixo(_,_,7,_,0).
verDiagonaisBaixo([],_,_,Encontrados,0) :- Encontrados < 4.
verDiagonaisBaixo([],_,_,Encontrados,1) :- Encontrados > 3.
verDiagonaisBaixo([H|T],Jogador,Objectivo,_,1) :- verDiagonal([H|T],Jogador,Objectivo,0,Enc),
                                                  Enc = 1.
verDiagonaisBaixo([H|T],Jogador,Objectivo,Encontrados,Quatro) :- verDiagonal([H|T],Jogador,Objectivo,0,Enc),
                                                                 Enc \= 1,
                                                                 verDiagonaisBaixo(T,Jogador,Objectivo,Encontrados,Quatro).

% verDiagonaisDireita(+Tab,+Jogador,+Objectivo,+Encontrados,-Quatro)
verDiagonaisDireita(_,_,7,_,0) :- !.
verDiagonaisDireita(Tab,Jogador,Objectivo,_,Quatro) :- verDiagonal(Tab,Jogador,Objectivo,0,Quatro),
                                                       Quatro = 1, !.
verDiagonaisDireita(Tab,Jogador,Objectivo,Encontrados,Quatro) :- Objectivo1 is Objectivo + 1,
                                                                 verDiagonal(Tab,Jogador,Objectivo,0,Enc),
                                                                 Enc \= 1,
                                                                 verDiagonaisDireita(Tab,Jogador,Objectivo1,Encontrados,Quatro).

% verDiagonal(+Tab,+Jogador,+Objectivo,+Encontrados,-Quatro)
verDiagonal([],_,_,_,0).
verDiagonal([],_,_,Encontrados,0) :- Encontrados < 4.
verDiagonal(_,_,_,Encontrados,1) :- Encontrados > 3.
verDiagonal([H|T],Jogador,Obj,Encontrados,Quatro) :- verDiag1(H,Jogador,0,Obj,Encontrados,Encontrou),
                                                     Encontrou = 1,
                                                     Encontrados1 is Encontrados + 1,
                                                     Obj1 is Obj + 1,
                                                     verDiagonal(T,Jogador,Obj1,Encontrados1,Quatro).
verDiagonal([H|T],Jogador,Obj,Encontrados,Quatro) :- verDiag1(H,Jogador,0,Obj,Encontrados,Encontrou),
                                                     Encontrou = 0,
                                                     Encontrados1 is 0,
                                                     Obj1 is Obj + 1,
                                                     verDiagonal(T,Jogador,Obj1,Encontrados1,Quatro).


% verDiag1(+Linha,+Jogador,+Current,+Obj,+Encontrados,-Encontrou).
verDiag1([],_,_,_,Encontrados,0) :- Encontrados < 4.
verDiag1([],_,_,_,Encontrados,1) :- Encontrados > 3.
verDiag1([H|_],Jogador,Obj,Obj,_,Encontrou) :- Jogador = H,
                                               Encontrou = 1.
verDiag1([H|_],Jogador,Obj,Obj,_,Encontrou) :- Jogador \= H,
                                               Encontrou = 0.
verDiag1([_|T],Jogador,Current,Obj,Encontrados,Encontrou) :- Current \= Obj,
                                                             Current1 is Current + 1,
                                                             verDiag1(T,Jogador,Current1,Obj,Encontrados,Encontrou).

%------------------------------------------------
% PARA AI MEDIO
%------------------------------------------------
%indica a peca na posicao dada
pecaAt([],_,_,-1).
pecaAt([H|_],X,1,Result) :- pecaAtLinha(H,X,_,Result).
pecaAt([_|T],X,Y,Result) :- Y1 is Y - 1,
                            pecaAt(T,X,Y1,Result).

pecaAtLinha([],_,_,-1).
pecaAtLinha([H|_],1,_,Result) :- Result = H.
pecaAtLinha([_|T],X,_,Result) :- X1 is X - 1,
                                 pecaAtLinha(T,X1,_,Result).
%----------------------
%encontra a peca do jogador dado a partir da posicao InicalX InicialY
encontraPeca([_|T],Jogador,InicialX,InicialY,CurrentY,X,Y) :- CurrentY < InicialY, !,
                                                              CurrentY1 is CurrentY + 1,
                                                              encontraPeca(T,Jogador,InicialX,InicialY,CurrentY1,X,Y).
encontraPeca([H|_],Jogador,InicialX,_,CurrentY,X,Y) :- encontrarPecaLinha(H,Jogador,InicialX,1,XEncontrado,Encontrou),
                                                       Encontrou = 1,
                                                       X = XEncontrado,
                                                       Y = CurrentY.
encontraPeca([H|T],Jogador,InicialX,InicialY,CurrentY,X,Y) :- encontrarPecaLinha(H,Jogador,InicialX,1,_,Encontrou),
                                                              Encontrou \= 1,
                                                              CurrentY1 is CurrentY + 1,
                                                              encontraPeca(T,Jogador,1,InicialY,CurrentY1,X,Y).

encontrarPecaLinha([],_,_,_,_,0).
encontrarPecaLinha([_|T],Jogador,Inicial,Current,X,Encontrou) :- Current < Inicial, !,
                                                                 Current1 is Current + 1,
                                                                 encontrarPecaLinha(T,Jogador,Inicial,Current1,X,Encontrou).
encontrarPecaLinha([H|_],Jogador,_,Current,X,Encontrou) :- H = Jogador,
                                                           X = Current,
                                                           Encontrou = 1.
encontrarPecaLinha([H|T],Jogador,Inicial,Current,X,Encontrou) :- H \= Jogador,
                                                                 Current1 is Current + 1,
                                                                 encontrarPecaLinha(T,Jogador,Inicial,Current1,X,Encontrou).

%gera um numero entre 1 e 4 ou 1 e 8 consoante a dificuldade pretendida
rand(Total,X) :- random(0,Total,X1),
                 X is X1+1.
%---------------------------------------------
%rand escolhe a direcao da jogada.
%Sentido dos ponteiros: 1-cima 2-direita 3-baixo 4-esquerda
%O ultimo é o counter, para ele tentar as opcoes todas e sair

%cima
jogaR4(Tab,NovoTab,Jogador,X,Y,1,_) :- Y1 is Y - 1,
                                       insere_pecaY(Y1,X,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,1,_) :- Y1 is Y - 1,
                                       pecaAt(Tab,X,Y1,Result),
                                       Result = Jogador,
                                       Y2 is Y1 - 1,
                                       insere_pecaY(Y2,X,Jogador,Tab,NovoTab).
%direita
jogaR4(Tab,NovoTab,Jogador,X,Y,2,_) :- X1 is X + 1,
                                       insere_pecaY(Y,X1,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,2,_) :- X1 is X + 1,
                                       pecaAt(Tab,X1,Y,Result),
                                       Result = Jogador,
                                       X2 is X1 + 1,
                                       insere_pecaY(Y,X2,Jogador,Tab,NovoTab).

%baixo
jogaR4(Tab,NovoTab,Jogador,X,Y,3,_) :- Y1 is Y + 1,
                                       insere_pecaY(Y1,X,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,3,_) :- Y1 is Y + 1,
                                       pecaAt(Tab,X,Y1,Result),
                                       Result = Jogador,
                                       Y2 is Y1 + 1,
                                       insere_pecaY(Y2,X,Jogador,Tab,NovoTab).

%esquerda
jogaR4(Tab,NovoTab,Jogador,X,Y,4,_) :- X1 is X - 1,
                                       insere_pecaY(Y,X1,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,4,_) :- X1 is X - 1,
                                       pecaAt(Tab,X1,Y,Result),
                                       Result = Jogador,
                                       X2 is X1 - 1,
                                       insere_pecaY(Y,X2,Jogador,Tab,NovoTab).

%diagonais
% 5-cimaDir 6-baixoDir 7-baixoEsq 8-cimaEsq

%cimaDir
jogaR4(Tab,NovoTab,Jogador,X,Y,5,_) :- Y1 is Y - 1,
                                       X1 is X + 1,
                                       insere_pecaY(Y1,X1,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,5,_) :- Y1 is Y - 1,
                                       X1 is X + 1,
                                       pecaAt(Tab,X1,Y1,Result),
                                       Result = Jogador,
                                       Y2 is Y1 - 1,
                                       X2 is X1 + 1,
                                       insere_pecaY(Y2,X2,Jogador,Tab,NovoTab).

%baidoDir
jogaR4(Tab,NovoTab,Jogador,X,Y,6,_) :- X1 is X + 1,
                                       Y1 is Y + 1,
                                       insere_pecaY(Y1,X1,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,6,_) :- X1 is X + 1,
                                       Y1 is Y + 1,
                                       pecaAt(Tab,X1,Y1,Result),
                                       Result = Jogador,
                                       X2 is X1 + 1,
                                       Y2 is Y1 + 1,
                                       insere_pecaY(Y2,X2,Jogador,Tab,NovoTab).

%baixoEsq
jogaR4(Tab,NovoTab,Jogador,X,Y,7,_) :- Y1 is Y + 1,
                                       X1 is X - 1,
                                       insere_pecaY(Y1,X1,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,7,_) :- Y1 is Y + 1,
                                       X1 is X - 1,
                                       pecaAt(Tab,X1,Y1,Result),
                                       Result = Jogador,
                                       Y2 is Y1 + 1,
                                       X2 is X1 - 1,
                                       insere_pecaY(Y2,X2,Jogador,Tab,NovoTab).

%cimaEsq
jogaR4(Tab,NovoTab,Jogador,X,Y,8,_) :- X1 is X - 1,
                                       Y1 is Y - 1,
                                       insere_pecaY(Y1,X1,Jogador,Tab,NovoTab).
jogaR4(Tab,NovoTab,Jogador,X,Y,8,_) :- X1 is X - 1,
                                       Y1 is Y - 1,
                                       pecaAt(Tab,X1,Y1,Result),
                                       Result = Jogador,
                                       X2 is X1 - 1,
                                       Y2 is Y1 - 1,
                                       insere_pecaY(Y2,X2,Jogador,Tab,NovoTab).

%Preenchido por adversario
jogaR4(Tab,NovoTab,Jogador,X,Y,RandomNumber,Counter) :- (RandomNumber > 7 -> RandomNumber1 is 5; RandomNumber1 is RandomNumber + 1),
                                                        Counter1 is Counter + 1,
                                                        Counter1 < 4,
                                                        jogaR4(Tab,NovoTab,Jogador,X,Y,RandomNumber1,Counter1).

%---------------------------------------------
%Se nao tiver nenhuma peca, ou essas tiverem todas bloqueadas o Consegui
%fica a 0
%Tipo = 4 joga so vertical e horizontal, Tipo=8 joga pra todo lad
joga_dificil(_,_,_,_,_,0).
joga_dificil(Tab,NovoTab,Jogador,Conseguiu,Tipo) :- joga_dificil(Tab,NovoTab,Jogador,1,1,Conseguiu,Tipo).
joga_dificil(Tab,NovoTab,Jogador,CurrentY,CurrentX,Consegui,Tipo) :- encontraPeca(Tab,Jogador,CurrentX,CurrentY,1,X,Y),!,
                                                                     rand(Tipo,RandN),
                                                                     (jogaR4(Tab,NovoTab,Jogador,X,Y,RandN,0) ->
								     ( Consegui = 1 ,
								      write('Inseri uma peça')) ;
								      X1 is X + 1,
								      joga_dificil(Tab,NovoTab,Jogador,Y,X1,Consegui)).
 joga_dificil(Tab,NovoTab,Jogador,CurrentY,CurrentX,_,_) :-       escolhe_num_aleatorio(X),
							    escolhe_num_aleatorio(Y),
							    insere_pecaY_a(Y,X,Jogador,Tab,NovoTab).

