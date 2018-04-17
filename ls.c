#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
	fmtname(char *path)
	{
	  static char buf[DIRSIZ+1];
	  char *p;
	  
	  // Find first character after last slash.
	  for(p=path+strlen(path); p >= path && *p != '/'; p--)
	    ;
	  p++;
	  if(strlen(p) >= DIRSIZ)
	    return p;
	  memmove(buf, p, strlen(p));
	  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	  return buf;
	}

void
	ls(char *path)
	{
	  char buf[512], *p;
	  int fd;
	  struct dirent de;
	  struct stat st;
	  
	  if((fd = open(path, 0)) < 0){
	    printf(2, "ls: cannot open %s\n", path);
	    return;
	  }
	  
	  if(fstat(fd, &st) < 0){
	    printf(2, "ls: cannot stat %s\n", path);
	    close(fd);
	    return;
	  }
	  
	  switch(st.type){
	  case T_FILE:
	    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
	    break;
	  
	  case T_DIR:
	    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
	      printf(1, "ls: path too long\n");
	      break;
	    }
	    strcpy(buf, path);
	    p = buf+strlen(buf);
	    *p++ = '/';
	    while(read(fd, &de, sizeof(de)) == sizeof(de)){
	      if(de.inum == 0)
	        continue;
	      memmove(p, de.name, DIRSIZ);
	      p[DIRSIZ] = 0;
	      if(stat(buf, &st) < 0){
	        printf(1, "ls: cannot stat %s\n", buf);
	        continue;
	      }
	      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
	    }
	    break;
	  }
	  close(fd);
	}

int strncmp( char * s1,char * s2, int n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int strbegin(char *a, char *b,int length)
{
   if(strncmp(a, b, length) == 0) return 1;
   return 0;
}
int memcmp(char* s1, char* s2,int n)
{	//printf(1,"hey");
    char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

	int strend(char *s, char *t)
{	//printf(1,"%c",s[4]);
    int ls = 0 ;
    ls = strlen(s); // find length of s
    //printf(1,"%d",ls);
    //printf(1,"hey");
    int lt = strlen(t); // find length of t
    //printf(1,"%d",lt);
    if (ls >= lt)  // check if t can fit in s
    {
        // point s to where t should start and compare the strings from there
        if(memcmp(t, s + (ls - lt), lt) == 0)
        	return 1;
    }
    return 0; // t was longer than s
}


	
	
	
	void
	ls_wild(char *path)
	{ char *wildcard_t = path;
	  wildcard_t++;
	  char *dir =".";
	  char buf[512],*p;
	  int fd;
	  struct dirent de;
	  struct stat st;
	  
	  if((fd = open(dir, 0)) < 0){
	    printf(2, "ls: cannot open %s\n",dir);
	    return;
	  }
	  
	  if(fstat(fd, &st) < 0){
	    printf(2, "ls: cannot stat %s\n", dir);
	    close(fd);
	    return;
	  }
	  int result;
	  switch(st.type){
	  case T_FILE:
	    result = strend(fmtname(dir), wildcard_t);
	    if(result != 0) //if result is 1 then sustring exists
	    	printf(1, "%s %d %d %d \n", fmtname(dir), st.type, st.ino, st.size);
	    	 break;
	    	
	  
	  case T_DIR:
	    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
	      printf(1, "ls: path too long\n");
	      break;
	    }
	    strcpy(buf, dir);
	    p = buf+strlen(buf);
	    *p++ = '/';
	    while(read(fd, &de, sizeof(de)) == sizeof(de)){
	      if(de.inum == 0)
	        continue;
	      memmove(p, de.name, DIRSIZ);
	      p[DIRSIZ] = 0;
	      if(stat(buf, &st) < 0){
	        printf(1, "ls: cannot stat %s\n", buf);
	        continue;
	      }
	      result = strend(buf,wildcard_t);
	      if(result!=0)
	      	printf(1, "%s %d %d %d \n", fmtname(buf), st.type, st.ino, st.size);
	    }
	    break;
	  }
	  close(fd);
	}
	
	void
	ls_wild_begin(char *path,int length)
	{ int ans =1,ans1 = 1;
	  char *wildcard_t = path;
	  wildcard_t[strlen(wildcard_t)-1] = 0;
	  ans = strcmp(wildcard_t,".");
	  ans1 = strcmp(wildcard_t,"..");
	  if(ans == 0)
	  	{ ls(".");
	  	  exit();
	  	 }
	  if(ans1 == 0)
	  {	ls("..");
	  	exit();
	  }
	  char *dir = ".";
	  char buf[512],*p;
	  int fd;
	  struct dirent de;
	  struct stat st;
	  
	  if((fd = open(dir, 0)) < 0){
	    printf(2, "ls: cannot open %s\n",dir);
	    return;
	  }
	  
	  if(fstat(fd, &st) < 0){
	    printf(2, "ls: cannot stat %s\n", dir);
	    close(fd);
	    return;
	  }
	  int result;
	  switch(st.type){
	  case T_FILE:
	    result = strbegin(fmtname(dir), wildcard_t,length);
	    if(result != 0)
	    	printf(1, "%s %d %d %d up\n", fmtname(dir), st.type, st.ino, st.size);
	    	 break;
	    	
	  
	  case T_DIR:
	    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
	      printf(1, "ls: path too long\n");
	      break;
	    }
	    strcpy(buf, dir);
	    p = buf+strlen(buf);
	    *p++ = '/';
	    while(read(fd, &de, sizeof(de)) == sizeof(de)){
	      if(de.inum == 0)
	        continue;
	      memmove(p, de.name, DIRSIZ);
	      p[DIRSIZ] = 0;
	      if(stat(buf, &st) < 0){
	        printf(1, "ls: cannot stat %s\n", buf);
	        continue;
	      }
	      result = strbegin(fmtname(buf),wildcard_t,length);
	      if(result!=0)
	      	printf(1, "%s %d %d %d down\n", fmtname(buf), st.type, st.ino, st.size);
	    }
	    break;
	  }
	  close(fd);
	}
	
	
	
	int
	main(int argc, char *argv[])
	{
	  
	  int i;
	  if(argc < 2){
	    ls(".");
	    exit();
	  }
	  char *arg;
	  arg = argv[1];
	  int len;
	  len = strlen(arg);
	  len = len - 1;
	 if(arg[0] == '*')
	  	{
	  		ls_wild(argv[1]);
	  		exit();
	  		
	  	}
	  if(arg[len] == '*')
	  	{ ls_wild_begin(argv[1],len);
	  	  exit();
	  	 }
	  for(i=1; i<argc; i++)
	    ls(argv[i]);
	  exit();
	}


