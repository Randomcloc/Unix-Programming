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
#include <pthread.h>
#include "matrices.h"
#include "serialization.h"
#include "deserialization.h"

#define PORTNUM "49137" /* Port number for server */
#define BACKLOG 128
#define BUFSIZE 5000

void *
handleRequest(void *input)
{
   // The struct is initialized and the values are extraced and put into variables.
   matrices workerInput = *(matrices *)input;
   int cfd = workerInput.socket;
   size_t n = workerInput.n;
   size_t p = workerInput.p;

   // This is where the buffer is set and a pointer to the buffer is initialized.
   // Total size of data = 2 * integers (n and p) + slice matrix size + B matrix size.
   unsigned char *array = malloc((2 * sizeof(int)) + ((n / p) * n * sizeof(double)) + (n * n * sizeof(double)));
   unsigned char *arrayPtr = array;

   // Serializing the data and storing in the buffer.
   arrayPtr = serializeInt(arrayPtr, n);
   arrayPtr = serializeInt(arrayPtr, p);

   printf("\nSending the following slice and matrix to localhost: %s\n\n", PORTNUM);
   printf("Slice: \n");

   for (size_t i = 0; i < (n / p); i++)
   {

      for (size_t j = 0; j < n; j++)
      {
         printf("%lf ", workerInput.slice[i][j]);
         arrayPtr = serializeDouble(arrayPtr, workerInput.slice[i][j]);
      }

      printf("\n");
   }

   printf("\nMatrix B: \n");

   for (size_t i = 0; i < n; i++)
   {

      for (size_t j = 0; j < n; j++)
      {
         printf("%lf ", workerInput.B[i][j]);
         arrayPtr = serializeDouble(arrayPtr, workerInput.B[i][j]);
      }

      printf("\n");
   }

   // Sending the buffer to the worker processes.
   size_t totWritten;
   unsigned const char *bufw = array;

   for (totWritten = 0; totWritten < BUFSIZE;)
   {
      ssize_t numWritten = send(cfd, bufw, BUFSIZE - totWritten, 0);
      if (numWritten <= 0)
      {
         if (numWritten == -1 && errno == EINTR)
            continue;
         else
         {
            fprintf(stderr, "Write error. errno %d.\n", errno);
         }
      }
      totWritten += numWritten;
      bufw += numWritten;
   }

   // Reading the data and storing in a buffer.
   unsigned char buf[BUFSIZE];
   size_t totRead;
   unsigned char *bufr = buf;

   for (totRead = 0; totRead < BUFSIZE;)
   {
      ssize_t numRead = read(cfd, bufr, BUFSIZE - totRead);
      if (numRead == 0)
         break;
      if (numRead == -1)
      {
         if (errno == EINTR)
            continue;
         else
         {
            fprintf(stderr, "Read error. errno %d.\n", errno);
         }
      }
      totRead += numRead;
      bufr += numRead;
   }

   printf("\nReceived the following slice resultant matrix: \n\n");

   // De-serializing the data from the buffer.
   // The struct result is initalized and memory is allocated.
   // A pointer to the buffer is also initialized.
   unsigned char *bufIn = buf;
   workerInput.res = getMatrices(n / p, n);

   for (size_t i = 0; i < n / p; i++)
   {

      for (size_t j = 0; j < n; j++)
      {

         workerInput.res[i][j] = deSerializeDouble(bufIn, &bufIn);
         printf("%lf ", workerInput.res[i][j]);
      }

      printf("\n");
   }

   // Setting the struct value to the local calculated result, since only the local variable was set before.
   ((matrices *)input)->res = workerInput.res;

   if (close(cfd) == -1) /* Close connection */
   {
      fprintf(stderr, "close error. errno %d.\n", errno);
   }

   pthread_exit(NULL);
   return NULL;
}

int main(int argc, char *argv[])
{
   size_t n = atoi(argv[1]);
   size_t p = atoi(argv[2]);

   double **A = getMatrices(n, n);
   double **B = getMatrices(n, n);

   matrices *workerMatrices = malloc(p * sizeof(matrices));

   // Can be used to get constant values for A and B.
   // Currently they are set to random values.

   // for (size_t i = 0; i < n; i++)
   // {

   //    for (size_t j = 0; j < n; j++)
   //    {

   //       A[i][j] = 2;
   //       B[i][j] = 2;
   //    }
   // }

   // The slices are taken from Matrix A and put into the struct slice array.
   // The values for n, p and B are also set in this loop.
   for (size_t i = 0; i < p; i++)
   {
      workerMatrices[i].slice = getMatrices(n / p, n);

      for (size_t j = 0; j < (n / p); j++)
      {

         for (size_t k = 0; k < n; k++)
         {

            workerMatrices[i].slice[j][k] = A[j + (n / p) * i][k];
         }
      }

      workerMatrices[i].B = B;
      workerMatrices[i].n = n;
      workerMatrices[i].p = p;
   }

   struct addrinfo hints;
   struct addrinfo *result, *rp;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_canonname = NULL;
   hints.ai_addr = NULL;
   hints.ai_next = NULL;
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

   if (getaddrinfo(NULL, PORTNUM, &hints, &result) != 0)
      exit(-1);

   int lfd, optval = 1;
   for (rp = result; rp != NULL; rp = rp->ai_next)
   {
      lfd = socket(rp->ai_family, rp->ai_socktype,
                   rp->ai_protocol);

      if (lfd == -1)
         continue; /* On error, try next address */

      if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR,
                     &optval, sizeof(optval)) == -1)
         exit(-1);

      if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
         break; /* Success */

      /* bind() failed, close this socket, try next address */
      close(lfd);
   }

   if (rp == NULL)
      exit(-1);

   {
      char host[NI_MAXHOST];
      char service[NI_MAXSERV];
      if (getnameinfo((struct sockaddr *)rp->ai_addr, rp->ai_addrlen,
                      host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
         fprintf(stdout, "Listening on (%s, %s)\n", host, service);
      else
         exit(-1);
   }

   // Printing matrices A and B.
   printf("\nMatrix A:\n");

   for (size_t i = 0; i < n; i++)
   {

      for (size_t j = 0; j < n; j++)
      {

         printf("%lf ", A[i][j]);
      }

      printf("\n");
   }

   printf("\nMatrix B:\n");

   for (size_t i = 0; i < n; i++)
   {

      for (size_t j = 0; j < n; j++)
      {

         printf("%lf ", B[i][j]);
      }

      printf("\n");
   }

   freeaddrinfo(result);

   if (listen(lfd, BACKLOG) == -1)
      exit(-1);

   pthread_attr_t tattr;
   pthread_attr_init(&tattr);
   pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
   pthread_t *threads = malloc(p * sizeof(pthread_t));

   for (size_t i = 0; i < p; i++)
   {

      int cfd = accept(lfd, NULL, NULL);

      if (cfd == -1)
      {
         continue; /* Print an error message */
      }

      workerMatrices[i].socket = cfd;
      pthread_create(threads + i, &tattr, handleRequest, &workerMatrices[i]);
   }

   // This loop makes sure that the server waits for the processes to finish.
   for (size_t k = 0; k < p; k++)
   {

      pthread_join(threads[k], NULL);
   }

   free(threads);

   //Merging the resultant slices and putting them in the matrix C.
   double **C = getMatrices(n, n);

   for (size_t process = 0; process < p; process++)
   {

      for (size_t i = 0; i < (n / p); i++)
      {

         for (size_t j = 0; j < n; j++)
         {

            double value = workerMatrices[process].res[i][j];
            C[i + (n / p) * process][j] = value;
         }
      }
   }

   // Printing the resultant matrix.
   printf("\nThe Resultant Matrix: \n\n");

   for (size_t i = 0; i < n; i++)
   {

      for (size_t j = 0; j < n; j++)
      {

         printf("%lf ", C[i][j]);
      }

      printf("\n");
   }

   printf("\n");

   pthread_attr_destroy(&tattr);
   free(workerMatrices);
   exit(EXIT_SUCCESS);
}