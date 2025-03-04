# sudo apt install libldap2-dev

CC=g++
CFLAG=-w -g3 -m64
CFLAG_DEBG=-g3 -m64 -L:./
LIBS=-fno-builtin -lrt -lpthread -lldap -llber

# 定义目标程序名称
APPNAME=ohMyCpp

# 源文件目录
VPATH=./src

# 对象文件存放目录
OBJDIR=objs
# 目标对象文件列表（每个 .o 都存放在 $(OBJDIR) 目录下）
OBJECTS=$(addprefix $(OBJDIR)/,\
	main.o fork.o io.o thread.o pNc.o philosopher.o shm.o\
)

# 生成目标程序
${APPNAME}: $(OBJECTS)
	$(CC) $(CFLAG) -o ${APPNAME} $(OBJECTS) $(LIBS)

debug:
	$(CC) $(CFLAG_DEBG) -o ${APPNAME} $(OBJECTS) $(LIBS)

# 确保 OBJDIR 目录存在
$(OBJDIR):
	mkdir -p $(OBJDIR)

# 规则：编译 .cpp 到 .o（带 objs/ 目录）
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) -c $(CFLAG) $< -o $@

# 清理编译生成的文件
clean:
	rm -rf $(OBJDIR) *.txt ${APPNAME}
