//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <mcp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
# include <windows.h>
# include <io.h>
# include <fcntl.h>
#include <errno.h>

#include "Common.h"

struct sockmap {
        char unix_path[108];
        unsigned short port;
};
#define MAYA_UNIX_SOCKET_SHARE_NAME "Maya_Unix_Socket_Share"
#define MAYA_UNIX_SOCKET_SHARE_SIZE 34*sizeof(struct sockmap)
#define MAXPATHLEN _MAX_PATH
#define MAXHOSTNAMELEN 64
#define bzero ZeroMemory
#define bcopy(s,d,n) memcpy(d,s,n)
#define readfd(handle, buffer, count) recv(handle, buffer, count, 0)
#define writefd(handle, buffer, count) send(handle, buffer, count, 0)
#define closefd(h)  closesocket(h)

SOCKET mcpOpen(char *name)
{
	return ConnectToMayaCommandPortByName(name);
	
#if 0
  char host[MAXHOSTNAMELEN], service[MAXHOSTNAMELEN];

  char buf[1024];

  char *cptr;
  int count, status;

  SOCKET sock_fd;
  struct hostent *hp;
  struct servent *sp;
  struct in_addr tmpaddr;
  BOOL optval;

  struct sockaddr_in	inet_a;  // inet_addr is a function name

  /*
   * Parse the name string.
   *
   *  A. If there is a ':' then it is an internet socket connection.
   *	1. Parse the name into <host>:<service> format.
   *	  a. If host is empty, use "localhost".
   *	  b. Lookup service with getservbyname().  If found, use
   *	     that port value.  If not found, attempt to convert
   *	     it to an integer and use that port value.
   *	2. Connect to <host>:<port>.  If the connection fails,
   *	   attempt to connect to <host>:tcpmux then request <service>
   *	   from tcpmux.
   *  B. Else the name is a unix domain socket name.
   *    1. If the first character is not '/', prepend '/tmp/' to the
   *	   name string.
   *	2. Connect to the socket name.
   */

  cptr = strchr(name, ':');

  if (cptr)
  {
    /* Copy the host name */
    if (cptr == name)
    {
      strcpy(host, "localhost");
    }
    else
    {
      strncpy(host, name, (cptr - name));
      host[cptr-name] = '\0';
    }

    /* Copy the service name */
    strcpy(service, cptr + 1);

    /* Initialize the inet_a struct */
    bzero((char *) &inet_a, sizeof(inet_a));
    inet_a.sin_family = AF_INET;

    /* Attempt to convert the host address as a dotted decimal number */
    tmpaddr.s_addr = inet_addr(host);
    status = (tmpaddr.s_addr != INADDR_NONE) ? 1 : 0;

	if (status == 1)
    {
      bcopy((char *) &tmpaddr, (char *) &inet_a.sin_addr, sizeof(tmpaddr));
    }
    else
    {
      /* Now find the host */
      hp = gethostbyname(host);

      if (!hp)
      {
	/* We could not find the host */
	errno = ENOENT;
	return -1;
      }

      bcopy(hp->h_addr, (char *) &inet_a.sin_addr, hp->h_length);
    }

    /* Attempt to decode the service parameter */
    sp = getservbyname(service, "tcp");

    if (sp)
    {
      inet_a.sin_port = sp->s_port;
    }
    else
    {
      /* Attempt to convert the service name as an integer */
      inet_a.sin_port = htons((unsigned short)atoi(service));
    }

    /* Create the socket */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
      return -1;
    }

    status = connect(sock_fd, (struct sockaddr *) &inet_a,
                     sizeof(inet_a));

    if (status < 0)
    {
      int status2;
      /*
       * The connection failed, try connecting to the tcpmux service.
       * tcpmux is at the well known port 1 so I just hardcoded it.
       */
      inet_a.sin_port = htons(1);

      status = errno;
      status2 = connect(sock_fd, (struct sockaddr *) &inet_a,
                        sizeof(inet_a));
      if (status2 < 0)
      {
		closefd(sock_fd);
		errno = status;
		return -1;
      }

      /*
       * We connected to tcpmux, now send the service name and see if
       * tcpmux can give it to us.  The tcpmux protocol works as follows:
       *
       *  1 Connect to the tcpmux port
       *  2 write the name of the service that you want followed by <CRLF>.
       *  3 read back the response terminated by <CRLF>
       *    a If the first character is a '+' then we are connected.
       *    b if the first character is a '-' then we are not connected.
       */
      sprintf(buf, "%s\r\n", service);
      status = mcpWrite(sock_fd, buf, (unsigned int) strlen(buf));
      if (status != (int)strlen(buf))
      {
		status = errno;
		closefd(sock_fd);
		errno = status;
		return -1;
      }

      /* Read up through the <CRLF> pair */
      /* There will be at least 3 characters coming back */
      cptr = buf;
      count = 3;
      while (1)
      {
		status = mcpRead(sock_fd, cptr, count);
		if (status <= 0)
		{
		  status = errno;
		  closefd(sock_fd);
		  errno = status;
		  return -1;
		}

		cptr += status;
		if (cptr[-1] == '\n')
		{
		  break;
		}
		else if (cptr[-1] == '\r')
		{
		  count = 1;
		}
		else
		{
		  count = 2;
		}
      }

      if (buf[0] != '+')
      {
		closefd(sock_fd);

		errno = WSAECONNREFUSED;
		WSASetLastError (WSAECONNREFUSED);

		return -1;
	  }
    }

    /*
     * We did it!  We actually succeeded in connecting to the name
     * via internet sockets.  Now make sure that we can send small
     * packets without delay.
     */
    optval = 1;
    status = setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval,
			sizeof(optval));
  }
  else /* This must be a unix domain socket name */
  {
	HANDLE h;
	void* pmem;
	DWORD *mask, bit;
	struct sockmap* map;
	int i;
	
	h = CreateFileMapping(INVALID_HANDLE_VALUE, 0, 
		PAGE_READWRITE,
		0, MAYA_UNIX_SOCKET_SHARE_SIZE,
		MAYA_UNIX_SOCKET_SHARE_NAME);
	if (!h)
		return -1;
	if (GetLastError() != ERROR_ALREADY_EXISTS)
	{
		/* no "unix" handles exist */
		CloseHandle(h);
		return -1;
	}
	
	pmem = MapViewOfFile(h, FILE_MAP_WRITE,	0, 0, 0);
	if (!pmem)
	{
		CloseHandle(h);
		return -1;
	}
	mask = (DWORD*)pmem;
	map = (struct sockmap*)((char*)pmem+sizeof(DWORD));

	/* check if name is already here */
	sock_fd = -1;
	for (bit = 1, i = 0; i < 32; ++i)
	{
		if (*mask & bit)
		{
			if (strcmp(map[i].unix_path, name) == 0)
				break; /* found */
		}
		bit = bit << 1;
	}
	
	/* found */
	if (i < 32)
	{
		/* Initialize the inet_a struct */
		bzero((char *) &inet_a, sizeof(inet_a));
		inet_a.sin_family = AF_INET;

		/* Now find the host */
		hp = gethostbyname("localhost");
		if (!hp)
			errno = ENOENT;
		else
		{
			bcopy(hp->h_addr, (char *) &inet_a.sin_addr, hp->h_length);
			inet_a.sin_port = map[i].port;

			/* Create the socket */
			sock_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (sock_fd >= 0)
			{
				status = connect(sock_fd, (struct sockaddr*)&inet_a, sizeof(inet_a));
				if (status < 0)
				{
					closefd(sock_fd);
					sock_fd = -1;
					errno = status;
				}
				else
				{
					optval = 1;
					status = setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY,
						(const char*)&optval, sizeof(optval));
				}
			}
		}
	}

	UnmapViewOfFile(pmem);
	CloseHandle(h);
  }

  /* Ok, we are connected.  Return the socket descriptor */
  return sock_fd;
#endif
}

// =========================================================================================

void mcpClose(SOCKET fd)
{
	DisconnectFromMayaCommandPort(fd);

#if 0
	closefd(fd);
#endif
}

// =========================================================================================

int mcpRead(SOCKET fd, void *ptr, unsigned int nbytes)
{
  char *buf = (char *) ptr;

  int nread = readfd(fd, buf, nbytes+1);
  
  return nread;
}

// =========================================================================================

int mcpWrite(SOCKET fd, void *ptr, unsigned int nbytes)
{
  unsigned int nleft;
  int nwritten;
  char *buf = (char *) ptr;

  nleft = nbytes;

  while (nleft > 0)
  {
    nwritten = writefd(fd, buf, nleft);

    if (nwritten <= 0)
    {
      return nwritten;		/* Error, return < 0 */
    }

    nleft -= nwritten;
    buf   += nwritten;
  }

  return nbytes - nleft;	/* Return nbytes */
}
