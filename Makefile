SRCS = $(addprefix ./SOURCE/, MSILMAgent.cpp MSILMParameters.cpp KirbyAgent.cpp KnowledgeBase.cpp Rule.cpp Element.cpp Dictionary.cpp IndexFactory.cpp Prefices.cpp LogBox.cpp MT19937.cpp)
DEPS = $(patsubst %.cpp,%.d,$(SRCS))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CFLAGS = -std=c++17 -MMD

ms: $(OBJS)
	${CXX} ${CFLAGS} ./SOURCE/MSILM_main.cpp ${OBJS} -o ./SOURCE/msilm.exe

%.o: %.cpp
	${CXX} ${CFLAGS} -o $@ -c $<

test: ms
	${CXX} ${CFLAGS} ./SOURCE/DistTest.cpp -o ./SOURCE/disttest.exe
	./SOURCE/disttest.exe
	${CXX} ${CFLAGS} ./SOURCE/KnwTest.cpp ${OBJS} -o ./SOURCE/knwtest.exe
	${CXX} ${CFLAGS} ./SOURCE/PrmTest.cpp ${OBJS} -o ./SOURCE/prmtest.exe

-include $(DEPS)

clean:
	rm -f ./SOURCE/*.o ./SOURCE/*.dump ./SOURCE/*.exe ./SOURCE/*.log ./SOURCE/*.rst
