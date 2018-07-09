SRCS = $(addprefix ./SOURCE/, MSILMAgent.cpp MSILMParameters.cpp KirbyAgent.cpp KnowledgeBase.cpp Rule.cpp Element.cpp Dictionary.cpp IndexFactory.cpp LogBox.cpp MT19937.cpp)
DEPS = $(patsubst %.cpp,%.d,$(SRCS))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CXXFLAGS = -std=c++17 -MMD

all: ms

%.o: %.cpp
	${CXX} ${CXXFLAGS} -o $@ -c $<

.PHONY: ms test clean

ms: $(OBJS)
	${CXX} ${CXXFLAGS} ./SOURCE/MSILM_main.cpp ${OBJS} -o ./SOURCE/msilm.exe

test: ms
	${CXX} ${CXXFLAGS} ./SOURCE/DistTest.cpp -o ./SOURCE/disttest.exe
	./SOURCE/disttest.exe
	${CXX} ${CXXFLAGS} ./SOURCE/KnwTest.cpp ${OBJS} -o ./SOURCE/knwtest.exe
	${CXX} ${CXXFLAGS} ./SOURCE/PrmTest.cpp ${OBJS} -o ./SOURCE/prmtest.exe

clean:
	rm -f ./SOURCE/*.o ./SOURCE/*.dump ./SOURCE/*.exe ./SOURCE/*.log ./SOURCE/*.rst

-include $(DEPS)
