/* ********************************************************************
 *   file:   client.cpp
 *   Simple sockets client
 * 
 * ********************************************************************
**/

/*
Students:
Moussa Lamreni

Moses Mbugua
*/
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#include <opencv2/opencv.hpp>

/* ********************************************************************
 * Variables
 * 
 **********************************************************************
**/
/* Client can connect to a server either on the local machine or across the network. */
#define PORT 4201
#define LOCAL "127.0.0.1"
#define REMOTE "192.168.1.50"
#define BUFLEN 30
#define WIDTH 640
#define HEIGHT 480
#define DEPTH 1
#define CHANNEL 3

int client_socket;
struct sockaddr_in client_addr;
char buffer[BUFLEN];
const char *server = REMOTE ;
int result, len;

timer_t timer = (timer_t) 0;

long int periode = 1000 ;	/* la période doit être en ms: below 70 ms it does not work */
int displaytime = 500 ; /* in ms*/
struct sigevent event;
struct itimerspec spec;

cv::Mat image = cv::Mat(HEIGHT, WIDTH, CV_8UC3) ;	

/* ********************************************************************
* Handler of timer
* 
*  ********************************************************************
*/


void handler_signal(int no)
{
	printf ("from Client: Request image\n");
	strcpy (buffer, "Request image\n");
	send (client_socket, buffer, BUFLEN, 0); //send (client_socket, buffer, strlen(buffer), 0);
	
	len= recv (client_socket, image.data, WIDTH*HEIGHT*DEPTH*CHANNEL, 0);
	
	cv::imshow ("Frame client", image) ;
    cv::waitKey(displaytime);
	

}

/* ********************************************************************
* Main thread
* 
* *********************************************************************
*/
int main (int argc, char *argv[])
{
	/* Allocate space for image display */
	cv::namedWindow ("Frame client", CV_WINDOW_AUTOSIZE) ;
	
	/* Create unnamed socket and then name it */
	client_socket = socket (AF_INET, SOCK_STREAM, 0);
	client_addr.sin_family = AF_INET;
	result = inet_aton (server, &client_addr.sin_addr);
	/* inet_aton: convert IP addresses from dots-and-number to abinary form and stores it in structure */
	if (result == 0)
	{
	perror ("inet_aton error");
	exit (1);
	}
	client_addr.sin_port = htons (PORT);
	
	/* Connect to the server */
	result = connect (client_socket, (struct sockaddr *) &client_addr, sizeof (client_addr));
	if (result < 0)
	{
	perror ("Client can't connect");
	exit (1);
	}

	/* Configurer le timer */
	signal (SIGRTMIN, handler_signal);
	event.sigev_notify = SIGEV_SIGNAL;
	event.sigev_signo  = SIGRTMIN;
	periode = periode*1000000 ;	/* conversion en ns */
	spec.it_interval.tv_sec  = periode/1000000000;
	spec.it_interval.tv_nsec = periode % 1000000000;	
	spec.it_value = spec.it_interval;
	/* Allouer le timer */
	if (timer_create(CLOCK_REALTIME, & event, & timer) != 0) {
		perror("timer_create");
		exit(EXIT_FAILURE);
	} 
	/* Programmer le timer */
	if (timer_settime(timer, 0, &spec, NULL) != 0) {
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
	
	/*  Pause until occurrence of timer*/
	while (1)
		pause() ;
	
	/* End */
	free (buffer) ;
	cv::destroyAllWindows() ;
	return 0;
}

