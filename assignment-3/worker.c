/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */
#define _BSD_SOURCE /* For NI_MAXHOST and NI_MAXSERV */
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include "matrices.h"
#include "serialization.h"
#include "deserialization.h"

#define PORTNUM "49137" /* Port number for server */
#define BUFSIZE 5000

int main(int argc, char *argv[])
{

   struct addrinfo hints;
   struct addrinfo *result, *rp;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_canonname = NULL;
   hints.ai_addr = NULL;
   hints.ai_next = NULL;
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_NUMERICSERV;

   if (getaddrinfo(argv[1], PORTNUM, &hints, &result) != 0)
      exit(-1);

   int cfd;
   for (rp = result; rp != NULL; rp = rp->ai_next)
   {
      cfd = socket(rp->ai_family, rp->ai_socktype,
                   rp->ai_protocol);

      if (cfd == -1)
         continue; /* On error, try next address */

      if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
         break; /* Success */

      /* close() failed, close this socket, try next address */
      close(cfd);
   }

   if (rp == NULL)
   {
      fprintf(stderr, "No socket to bind...\n");
      exit(-1);
   }

   freeaddrinfo(result);

   {

      unsigned char buf[BUFSIZE];
      size_t totRead;
      unsigned char *bufr = buf;

      for (totRead = 0; totRead < BUFSIZE;)
      {
         ssize_t numRead = recv(cfd, bufr, BUFSIZE - totRead, 0);

         if (numRead == 0)
            break;
         if (numRead == -1)
         {
            if (errno == EINTR)
               continue;
            else
            {
               fprintf(stderr, "Read error.\n");
            }
         }
         totRead += numRead;
         bufr += numRead;
      }
      printf("Received the slice and matrix B: \n");

      //Reading and de-serializing the data from the buffer.
      //bufOut is a pointer to the buffer.
      unsigned char *bufOut = buf;

      size_t n = deSerializeInt(bufOut, &bufOut);
      size_t p = deSerializeInt(bufOut, &bufOut);

      double **slice = getMatrices(n / p, n);
      double **B = getMatrices(n, n);
      double **resultantSlice = getMatrices(n / p, n);

      printf("Values for n and p: n = %zu, p = %zu\n\n", n, p);
      printf("Slice: \n");

      for (size_t i = 0; i < n / p; i++)
      {

         for (size_t j = 0; j < n; j++)
         {

            slice[i][j] = deSerializeDouble(bufOut, &bufOut);

            printf("%lf ", slice[i][j]);
         }

         printf("\n");
      }

      printf("\n");
      printf("Matrix B: \n");

      for (size_t i = 0; i < n; i++)
      {

         for (size_t j = 0; j < n; j++)
         {

            B[i][j] = deSerializeDouble(bufOut, &bufOut);

            printf("%lf ", B[i][j]);
         }

         printf("\n");
      }
   
      //The resultant slice is calculated by multiplying the received slice and matrix B.
      //A buffer and a pointer to the buffer are initialized to store the serialized resultant slice.
      resultantSlice = multiply(slice, B, n, p);
      unsigned char *output = malloc((n / p) * n * sizeof(double));
      unsigned char *outputPtr = output;

      //Serializing the resultant slice.
      for (size_t i = 0; i < (n / p); i++)
      {

         for (size_t j = 0; j < n; j++)
         {

            outputPtr = serializeDouble(outputPtr, resultantSlice[i][j]);
         }
      }

      //Sending the buffer to the master process.
      size_t totWritten;
      unsigned char *bufw = output;
      for (totWritten = 0; totWritten < BUFSIZE;)
      {
         ssize_t numWritten = send(cfd, bufw, BUFSIZE - totWritten, 0);
         if (numWritten <= 0)
         {
            if (numWritten == -1 && errno == EINTR)
               continue;
            else
            {
               fprintf(stderr, "Write error.\n");
               exit(EXIT_FAILURE);
            }
         }
         totWritten += numWritten;
         bufw += numWritten;
      }

      //Printing the resultant slice.
      printf("\nSending the following resultant slice matrix to localhost: %s\n\n", PORTNUM);

      for (size_t i = 0; i < n / p; i++)
      {

         for (size_t j = 0; j < n; j++)
         {

            printf("%lf ", resultantSlice[i][j]);
         }

         printf("\n");
      }
   }
   printf("\n");

   if (close(cfd) == -1) /* Close connection */
   {
      fprintf(stderr, "close error.\n");
      exit(EXIT_FAILURE);
   }

   exit(EXIT_SUCCESS);
}