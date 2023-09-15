#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ulimit.h>
#include <sys/resource.h>

//long int ulimit(int cmd, long newlimit);
//int getopt (int argc, char *const *argv, const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

//gid_t getgid(void);
//gid_t getegid(void);
//uid_t getuid(void);
//uid_t geteuid(void);
//
//pid_t getpid();
//pid_t getppid();
//pid_t getpgid(pid_t pid);
//int setpgid(pid_t pid, pid_t pgid);

extern char** environ;
struct rlimit rlp;

int main(int argc, char* argv[])
{
	char options[] = "vV:disupU:cC:";  /* valid options */
	     int c, invalid = 0, dflg = 0, fflg = 0, gflg = 0;
	     char* f_ptr, * g_ptr;
		 long unew;
		 int argc_cpy = argc;
		 char* tmp;
		 if (argc == 1) {
			 printf("Invalid arg num");
			 exit(1);
		 }
		 while (argc_cpy > 1) {
			 tmp = argv[argc - argc_cpy + 1];
			 argv[argc - argc_cpy + 1] = argv[argc_cpy - 1];
			 argv[argc_cpy - 1] = tmp;
			 argc_cpy--;
		 }
		     printf("argc equals %d\n", argc);
	     while ((c = getopt(argc, argv, options)) != EOF) {
			 switch (c) {
			 case 'i':
				 printf("Effective group id: %d\n", getegid());
				 printf("Real group id: %d\n", getgid());
				 printf("Effective user id: %d\n", geteuid());
				 printf("Real user id: %d\n", getuid());
				 break;
			 case 's':
				 if (setpgid(getpid(), getpgid(getpid())) == 0) {
					 printf("Succesfully set process as a group leader\n");
				 }
				 break;
			 case 'p':
				 printf("%ld - process id\n%ld - parent id\n %ld - group id\n", getpid(), getppid(), getpgid(getpid()));
				 printf("group process number = %ld\n", getpgid(0));
				 break;
			 case 'u':
				 printf("%ld - ulimit\n", ulimit(UL_GETFSIZE));
				 break;
			 case 'U':
				 unew = atol(optarg);
				 printf("%ld - old ulimit\n", ulimit(UL_GETFSIZE));
				 int a = ulimit(UL_SETFSIZE, unew);
				 if (a == -1) {
					 fprintf(stderr, "Error, too big argument to ulimit\n");
				 }
				 printf("%ld - new ulimit\n", ulimit(UL_GETFSIZE));
				 break;
			 case 'c':
				 getrlimit(RLIMIT_CORE, &rlp);
				 printf("%ld - core\n", rlp.rlim_cur);
				 break;
			 case 'C':
				 getrlimit(RLIMIT_CORE, &rlp);
				 rlp.rlim_cur = atol(optarg);
				 int lim = setrlimit(RLIMIT_CORE, &rlp);
				 if (lim == -1) {
					 fprintf(stderr, "Error, too big argument for core\n");
				 }
				  break;
		      case 'd':
				  printf("Current directory:\n%s\n", getcwd(NULL, 200));;
				break;
			  case 'v':
				  printf("Variables:\n");
				  for (char** i = environ; *i != NULL; i++)
					  printf("%s\n", *i);
				  break;
			  case 'V':

				  if (putenv(optarg) != 0)
					  fprintf(stderr, "Error, can't change argument");
				  break;
		      case '?':
			          printf("invalid option is %c\n", optopt);
			          invalid++;
			
		}
		
	}
	     printf("invalid equals %d\n", invalid);
	     printf("optind equals %d\n", optind);
	     if (optind < argc)
		      printf("next parameter = %s\n", argv[optind]);
	 }