CXX = g++
CFLAGS = -g -O2 -DDEBUG -fPIC -std=c++0x #-Wall

LFLAGS = -lpthread -lglut -lGL -lfreeimage

OBJS = Utils.o Grid.o GlutClass.o Simulation.o Order.o Agent.o Node.o Planner.o Wave.o LowLevelSolver.o HighLevelSolver.o TreeNode.o main.o 

MKDIR_P = mkdir -p
OUT_DIR = build
PREFIX_OBJS = $(patsubst %.o,${OUT_DIR}/%.o,$(OBJS))

EXEC = program

all: ${OUT_DIR} $(EXEC)

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

%.o: src/%.cpp $(DEPS)
	@echo "Compilando $@"
	@$(CXX) $(CFLAGS) -c $< -o ${OUT_DIR}/$@

$(EXEC): $(OBJS)
	@echo "\nLinkando $(EXEC)\n"
	@$(CXX) -o ${OUT_DIR}/$(EXEC) $(PREFIX_OBJS) $(LFLAGS)

clean:
	@echo "Limpando..."
	@rm -f $(PREFIX_OBJS) ${OUT_DIR}/$(EXEC) *~

