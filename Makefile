ID = -I/usr/local/include
LD = -L/usr/local/lib
LIBS = -lboost_program_options
SOURCEDIR = ./SOURCE
OBJ = MSILMAgent.o MSILMParameters.o KirbyAgent.o KnowledgeBase.o Rule.o Element.o Dictionary.o IndexFactory.o Prefices.o LogBox.o Parameters.o MT19937.o
OBJS = $(addprefix ${SOURCEDIR}/, $(OBJ))
HD = Distance.hpp
HDS = $(addprefix ${SOURCEDIR}/, $(HD))
OPT = --std=c++17 -O2

ms: ${OBJS}
	${CXX} ${OPT} ${SOURCEDIR}/MSILM_main.cpp ${OBJS} ${LD} ${LIBS} -o ${SOURCEDIR}/msilm.exe

$(SOURCEDIR)/%.o: $(SOURCEDIR)/%.cpp
	@[ -d $(SOURCEDIR/) ]
	${CXX} ${OPT} ${ID} ${LD} ${LIBS} -o $@ -c $<

test: ms
	${CXX} ${OPT} ${SOURCEDIR}/DistTest.cpp -o ${SOURCEDIR}/disttest.exe
	${SOURCEDIR}/disttest.exe

$(SOURCEDIR)/MSILM_main.cpp: ${OBJS} MSILM_main.h
$(SOURCEDIR)/MSILMAgent.o: $(SOURCEDIR)/KirbyAgent.o $(SOURCEDIR)/MT19937.o $(SOURCEDIR)/MSILMAgent.h
$(SOURCEDIR)/KirbyAgent.o: $(SOURCEDIR)/KnowledgeBase.o $(SOURCEDIR)/LogBox.o $(SOURCEDIR)/KirbyAgent.h
$(SOURCEDIR)/KnowledgeBase.o: ${HDS} $(SOURCEDIR)/Rule.o $(SOURCEDIR)/IndexFactory.o $(SOURCEDIR)/Prefices.o $(SOURCEDIR)/LogBox.o $(SOURCEDIR)/MT19937.o $(SOURCEDIR)/KnowledgeBase.h
$(SOURCEDIR)/Rule.o: $(SOURCEDIR)/Element.o $(SOURCEDIR)/Dictionary.o $(SOURCEDIR)/IndexFactory.o $(SOURCEDIR)/Prefices.o $(SOURCEDIR)/Rule.h
$(SOURCEDIR)/Element.o: $(SOURCEDIR)/Dictionary.o $(SOURCEDIR)/IndexFactory.o $(SOURCEDIR)/Prefices.o $(SOURCEDIR)/Element.h
$(SOURCEDIR)/Dictionary.o: $(SOURCEDIR)/Dictionary.h
$(SOURCEDIR)/IndexFactory.o: $(SOURCEDIR)/IndexFactory.h
$(SOURCEDIR)/Prefices.o: $(SOURCEDIR)/Prefices.h
$(SOURCEDIR)/LogBox.o: $(SOURCEDIR)/LogBox.h
$(SOURCEDIR)/MT19937.o: $(SOURCEDIR)/MT19937.h
$(SOURCEDIR)/MSILMParameters: $(SOURCEDIR)/Parameters.o $(SOURCEDIR)/MSILMParameters.h
$(SOURCEDIR)/Parameters.o: $(SOURCEDIR)/Parameters.h

clean:
	rm -f ${SOURCEDIR}/*.o ${SOURCEDIR}/*.dump ${SOURCEDIR}/*.exe ${SOURCEDIR}/*.log ${SOURCEDIR}/*.rst