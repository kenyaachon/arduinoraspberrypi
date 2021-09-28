/* ********************************************************************
* file:   netserver.cpp
* Simple sockets server
*/

/*
Students:
Moussa Lamreni

Moses Mbugua
*/
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <opencv2/opencv.hpp> 

/* ********************************************************************
* Variables
*/
#define PORT 4201
#define SERVER "192.168.1.50"
#define BUFLEN 30
#define WIDTH 640
#define HEIGHT 480
#define DEPTH 1
#define CHANNEL 3

int server_socket, client_socket, client_len;
struct sockaddr_in server_addr, client_addr;
char buffer[BUFLEN];
char ch ;
int len, result;
cv::Mat image = cv::Mat(HEIGHT, WIDTH, CV_8UC3) ;	/* Allocate a 3-channel WIDTH x HEIGHT image */
int displaytime = 1000 ;		/* Time in ms for image display */

/* ********************************************************************
* Main thread
*/
int main (void)
{
	/* Open image source */
    cv::VideoCapture vcap(0);	
    if(!vcap.isOpened()) {
    printf ( "Error opening camera \n");
    exit (1) ;
    }
    
    //Test image acquisition 
    cv::namedWindow ("Frame server", CV_WINDOW_AUTOSIZE) ; //Allocate space for image display
    vcap >> image ;
    printf ( "image type: %d \n", image.type() ) ;
    printf ( "image depth: %d \n", image.depth()  ) ;
    printf ( "image channels: %d \n", image.channels()  ) ;
    printf ( "image width: %d \n", image.size().width ) ;
    printf ( "image height: %d \n", image.size().height ) ; 
    cv::imshow ("Frame server", image) ;
    cv::waitKey(displaytime);
    cv::destroyAllWindows() ;
	
	/*Create unnamed socket and give it a "name" */
    server_socket = socket (PF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    result = inet_aton (SERVER, &server_addr.sin_addr);
	/* inet_aton: convert IP addresses from dots-and-number to abinary form and stores it in structure */
    if (result == 0)
    {
        printf ("inet_aton failed\n");
        exit (1);
    }
    server_addr.sin_port = htons (PORT);

	/* Bind to the socket */
    result = bind (server_socket, (struct sockaddr *) &server_addr, sizeof (server_addr));
    if (result != 0)
    {
        perror ("bind");
        exit (1);
    }

	/* Create a client queue */
    result = listen (server_socket, 1);
    if (result != 0)
    {
        perror ("listen");
        exit (1);
    }
    printf ("Network server running\n");

	/* Accept a connection */
    client_len = sizeof (client_addr);
    client_socket = accept (server_socket, (struct sockaddr *) &client_addr, (socklen_t * __restrict__)&client_len);

    printf ("Connection established to %s\n", inet_ntoa (client_addr.sin_addr));
	/* inet_ntoa: convert IP addresses from dots-and-number to a struct in_addr and back */

    // strcpy (buffer, "Server> ");
    do
    {
	
	vcap >> image;
	usleep(40000);
	printf ("from Client : ");
	len = recv (client_socket, buffer, BUFLEN, 0);	/* last byte for zero*/
	puts (buffer);
	printf ("from Server : Send image\n ");
	send(client_socket, image.data, WIDTH*HEIGHT*DEPTH*CHANNEL, 0);
    }
    while (1);


    close (client_socket);
    close (server_socket);
    printf ("Connection terminated.  Server shutting down\n") ;
    vcap.release();
    return 0;
}

