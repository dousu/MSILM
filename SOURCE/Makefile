OUTPUT = a.out
LD = -L/usr/local/lib
LIBS = -lboost_serialization -lboost_thread -lboost_system -lboost_program_options
OBJ = MSILMAgent.o KirbyAgent.o KnowledgeBase.o Rule.o Element.o Dictionary.o IndexFactory.o Prefices.o LogBox.o Parameters.o MT19937.o MSILMParameters.o
HD = Distance.hpp
OPT = -g -O2

ms: ${OBJ}
	${CXX} ${OPT} MSILM_main.cpp ${OBJ} ${HD} ${LD} ${LIBS} -o msilm.exe

.cpp.o:
	${CXX} ${OPT} -c $< ${ID} ${LD} ${LIBS2}

KirbyAgent.o: KnowledgeBase.o LogBox.o KirbyAgent.h
KnowledgeBase.o: ${HD} Rule.o IndexFactory.o Prefices.o LogBox.o KnowledgeBase.h
Rule.o: Element.o Dictionary.o IndexFactory.o Prefices.o Rule.h
Element.o:Dictionary.o IndexFactory.o Prefices.o Element.h
Dictionary.o:Dictionary.h
IndexFactory.o:IndexFactory.h
Prefices.o:Prefices.h
LogBox.o:LogBox.h
MT19937.o:MT19937.h
Parameters.o:Parameters.h
MSILMParameters.o:Parameters.o MSILMParameters.h
MSILMAgent.o:KirbyAgent.o MSILMAgent.h
MSILM_main.cpp: MSILMAgent.o Rule.o Element.o MT19937.o Dictionary.o LogBox.o MSILMParameters.o MSILM_main.h

clean:
	rm -f *.o *.dump *.exe
