# Dodanie dodatkowych rozszerzen
.SUFFIXES: .c .cpp .o .x .h 

# aktualna lokalizacja
DIR = `basename $(CURDIR)`

#########################################################################

###################     PRZYPISANIE NAZW PLIKOW     #####################

#########################################################################


# nazwy plikow 
NAME1 = 4

# nazwy plikow wykonywalnych
EXEC1    = server.x 

# obiekt biblioteki
OBJS1    = $(NAME1).o





# Pliki naglowkowe
# HEADS3 = $(NAME3).h
# HEADS4 = $(NAME4).h



#########################################################################

########     UNIWERSALNY WZORZEC TWORZENIA PLIKOW OBIEKTOWYCH    ########

#########################################################################

# INFO:
# Znak % oznacza wzorzec, tzn. dowolny ciag znakow, ktore tworza rdzen nazwy 
# pliku. 
# Zmienna wewnetrzna $< oznacza pierwszy prerekwizyt, tu: nazwe odpowiedniego 
# pliku z rozszerzeniem .c, ktory nalezy uaktualnic aby otrzymac plik .o. 

# tworzy akutalnie wymagane pliki obiektowe OBJS#
%.o: %.cpp %.h
	$(CO) $(CFLAGS) -c $<
# Jak wyzej, ale bez zaleznosci od plikow naglowkowych
%.o: %.cpp
	$(CO) $(CFLAGS) -c $<



# eliminacja ewentualnej mozliwosci wykonania pliku "all"
.PHONY: all

# aktualizacja wszystkich plikow wykonywalnych
all: $(EXEC1) 



#########################################################################

########################     URUCHAMIANIE     ###########################

#########################################################################


# Uruchomienie 
run: $(EXEC1)
	./server.x 
 

#########################################################################

#####################     TWORZENIE BIBLIOTEK     #######################

#########################################################################


# wykorzystywany kompliator
CO = g++
#wykorzystywany linker
LD = $(CO)

# flagi kompilatora wykorzystywane podczas kompilacji 
CFLAGS = -Wall -pedantic -std=c++17


#########################################################################

####################     KOMPILACJA PLIKOW .x     #######################

#########################################################################


# Main_array.x
$(EXEC1): $(OBJS1)  
	$(CO) $(OBJS1) $(CFLAGS) -o $(EXEC1) 



#########################################################################

###########################     RESZTA     ##############################

#########################################################################

# eliminacja ewentualnej mozliwosci wykonania pliku "clean" lub "tar" 
.PHONY: clean tar

# funkcja usuwajaca zbedne elementy o danych rozszerzeniach/nazwach
clean:                                                     
	rm -f *.o *.a *.x a.out ./lib/*.a ./lib/*.so;


# funkcja archiwizujaca wszystkie pliki po pozbyciu sie zbednych
# archiwum o rozszerzeniu .tar.gz i nazwia katalogu w ktorym sie znajduje
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )

