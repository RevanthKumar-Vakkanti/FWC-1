#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "libs/matfun.h"
#include "libs/geofun.h"
#define PORT 5500
#define BUFFER_SIZE 8192

void sendHTMLForm(int client_fd, double IJ, double JK, double KI,double lx, double ly, double IJx, double IJy, double JKx, double JKy, double KIx, double KIy, double Ax, double Ay, double Bx, double By, double Cx, double Cy, double Gx, double Gy, double AIJ, double AJK, double AKI) {
    char *html_template =  "<!DOCTYPE html>"
                           "<html lang=\"en\">"
                           "<head>"
                           "<meta charset=\"UTF-8\">"
                           "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                           "<title>Mat_Alg</title>"
                           
                           "<style>"
                           "body {font-family: 'Times New Roman', Times, serif; text-align: left;}"
    			   "h1 {font-size: 2.0rem; color: red;}"
			   "form {margin: 20px auto; max-width: 400px;}"
    			   "label {display: block; margin-bottom: 5px;}"
    			   "input[type=\"text\"] {width: 100px; padding: 5px; margin-bottom: 10px;}"
   			   "button {padding: 10px 20px; background-color: #00ff7b; color: #fff; border: none; cursor: pointer;}"
    			   "button:hover {background-color: #0056b3;}"
    			   ".results {margin-top: 20px;}"
			   "</style>"
			   
			   "</head>"			   
                           "<body>"
                           "<h1>Mat_Alg</h1>"
                           
                           "<form method=\"post\">"
         		   "<label for=\"p1\">Vertex I: (p1, q1)</label><br>"
                           "<input type=\"text\" id=\"p1\" name=\"p1\" value=\"1\" step=\"any\" required>"
      			   "<input type=\"text\" id=\"q1\" name=\"q1\" value=\"-1\" step=\"any\" required><br><br>"
                           "<label for=\"p2\">Vertex J: (p2, q2)</label><br>"
                           "<input type=\"text\" id=\"p2\" name=\"p2\" value=\"-4\" step=\"any\" required>"
                           "<input type=\"text\" id=\"q2\" name=\"q2\" value=\"6\" step=\"any\" required><br><br>"
                           "<label for=\"p3\">Vertex K: (p3, q3)</label><br>"
                           "<input type=\"text\" id=\"p3\" name=\"p3\" value=\"-3\" step=\"any\" required>"
     			   "<input type=\"text\" id=\"q3\" name=\"q3\" value=\"-5\" step=\"any\" required><br><br>"
                           "<button type=\"submit\">Calculate</button>"
                           "</form>"
                           "<div class=\"results\">"
                           "<h3>Results</h3>"
                        
                          "<h4>*******Orthocenter*******</h4>"
                          "<p>Orthocenter L: %lf, %lf</p>\n"
                          "<h4>****Perpendicular Bisector*****</h4>"
                          "<p>Bisector of IJ:%.6f,%.6f</p>\n"
                          "<p>Bisector of JK:%.6f,%.6f</p>\n"
                          "<p>Bisector of KI:%.6f,%.6f</p>\n"
                          "<h4>*******Medians*******</h4>"
                          "<p>Median IA: %.6f</p>"
                          "<p>Median JB: %.6f</p>"
                          "<p>Median KC: %.6f</p>"                        
                          "<h4>*****Angular Bisector******</h4>"
                          "<p></p>"
                          "<p>Bisector of I : %.6f, %.6f</p>"
                          "<p>Bisector of J: %.6f, %.6f</p>"
                          "<p>Bisector of K: %.6f, %.6f</p>"
                          "<p>Distance IJ: %.6f</p>"
                          "<p>Distance JK: %.6f</p>"
                          "<p>Distance KI: %.6f</p>"
                          "<p>Incenter G: %.6f, %.6f</p>"
                          
  		          "</div>"
                          "</body>"
                          "</html>";
                          
    char response[BUFFER_SIZE];
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    snprintf(response + strlen(response), BUFFER_SIZE - strlen(response), html_template, IJ,JK,KI,lx, ly,IJx,IJy,JKx,JKy,KIx,KIy,Ax,Ay,Bx,By,Cx,Cy,Gx,Gy,AIJ,AJK,AKI);

    send(client_fd, response, strlen(response), 0);
}

int main() {	
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Print server address
    printf("Server is listening on port localhost:5500\n");

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted\n");
        
        char buffer[BUFFER_SIZE] ={0};
        read(client_fd, buffer, BUFFER_SIZE);
        printf("Received data from client: %s\n", buffer);
        
        char *body_start = strstr(buffer,"\r\n\r\n");
        
        if(body_start){
        
            body_start +=4;

    int p1, q1, p2, q2, p3, q3, row = 2, col = 1;
    
 /*   printf("Enter point I's coordinates here:");
    scanf("%d %d", &p1, &q1);
    printf("Enter point J's coordinates here:");
    scanf("%d %d", &p2, &q2);
    printf("Enter point K's coordinates here:");
    scanf("%d %d", &p3, &q3); */
    
    double **I, **J, **K, **m1, **m2, **m3, **L, **N, **m3_t, **m1_t, **R0, **R1, **R; 
    sscanf(body_start,"p1=%d&q1=%d&p2=%d&q2=%d&p3=%d&q3=%d  ", &p1, &q1, &p2, &q2, &p3, &q3);

    I = createMat(row, col);
    J = createMat(row, col);
    K = createMat(row, col);

    I[0][0] = p1;
    I[1][0] = q1;
    J[0][0] = p2;
    J[1][0] = q2;
    K[0][0] = p3;
    K[1][0] = q3;

    // Perform matrix operations
    m1 = Matsub(I, J, row, col);
    m2 = Matsub(J, K, row, col);
    m3 = Matsub(K, I, row, col);
    N = transposeMat(combineMat(m3, m1, row, col), row, row);
    m3_t = transposeMat(m3, row, col);
    m1_t = transposeMat(m1, row, col);
    R0 = Matmul(m3_t, J, col, row, col);
    R1 = Matmul(m1_t, K, col, row, col);
    R = createMat(2, 1);
    R[0][0] = R0[0][0];
    R[1][0] = R1[0][0];

    // Solving the linear equations
    L = createMat(2, 1);
    L[1][0] = (-1) * (R[0][0] * N[1][0] - N[0][0] * R[1][0]) / Matdet(N);
    L[0][0] = (R[0][0] * N[1][1] - N[0][1] * R[1][0]) / Matdet(N);
   // printf("The sources of linear equation are L1,L2: %lf, %lf\n\n", L[1][0], L[0][0]);

    // Perpendicular bisector
    double **sij, **sjk, **ski, **midpoint_IJ, **midpoint_JK, **midpoint_KI;
    sij = Matsub(I, J, row, col);
    sjk = Matsub(J, K, row, col);
    ski = Matsub(K, I, row, col);
    midpoint_IJ = normVec(sij);
    midpoint_JK = normVec(sjk);
    midpoint_KI = normVec(ski);
   /* printf("Perpendicular Bisectors of IJ, JK, KI are : {%lf, %lf}, {%lf, %lf}, {%lf, %lf}\n\n", midpoint_IJ[0][0], midpoint_IJ[1][0], midpoint_JK[0][0], midpoint_JK[1][0], midpoint_KI[0][0], midpoint_KI[1][0]); */
 
    // Medians
    double **A, **B, **C, **side_ij, **side_jk, **side_ki;
    A = Matsec(J, K, 2, 1.0);
    B = Matsec(K, I, 2, 1.0);
    C = Matsec(I, J, 2, 1.0);
    side_ij = Matsub(I, A, row, col);
    side_jk = Matsub(J, B, row, col); 
    side_ki = Matsub(K, C, row, col);  
    double sideIJ = Matnorm(side_ij, row);  
    double sideJK = Matnorm(side_jk, row);
    double sideKI = Matnorm(side_ki, row);
    // printf("Length of Medians IA, JB, KC are :%lf, %lf, %lf\n\n", sideIJ, sideJK, sideKI);

    // Angular bisector
    int rows = 2, cols = 1;
    double **G, **Aa, **Ba, **Ca, **difference_IJ, **difference_JK, **difference_IK;
    G = createMat(rows, cols);                           
    difference_IJ = Matsub(J, I, rows, cols); 
    difference_JK = Matsub(K, J, rows, cols); 
    difference_IK = Matsub(K, I, rows, cols);
    double distance_IJ = Matnorm(difference_IJ, rows);                        
    double distance_JK = Matnorm(difference_JK, rows);
    double distance_IK = Matnorm(difference_IK, rows); 
    double k = distance_IJ;
    double i = distance_JK;
    double j = distance_IK;
    //printf("Dismensions of the sides IJ, JK, IK are : %lf, %lf, %lf\n\n", i, j, k);

    double l1 = (i + k - j) / 2;
    double l2 = (i + j - k) / 2;
    double l3 = (k + j - i) / 2;
    Aa = Matscale(Matadd(Matscale(K, 2, 1, l1), Matscale(J, 2, 1, l2), 2, 1), 2, 1, 1.0 / (l1 + l2));
    Ba = Matscale(Matadd(Matscale(I, 2, 1, l2), Matscale(K, 2, 1, l3), 2, 1), 2, 1, 1.0 / (l2 + l3));
    Ca = Matscale(Matadd(Matscale(J, 2, 1, l3), Matscale(I, 2, 1, l1), 2, 1), 2, 1, 1.0 / (l3 + l1));
    //printf("Angular Bisectors  Aa, Ba, Ca : {%lf, %lf}, {%lf, %lf}, {%lf, %lf}\n\n", Aa[0][0], Aa[1][0], Ba[0][0], Ba[1][0], Ca[0][0], Ca[1][0]);

    double **temp_1 = Matscale(I, 2, 1, i);
    double **temp_2 = Matscale(J, 2, 1, j);
    double **temp_3 = Matscale(K, 2, 1, k);
    double **eigenvalues = Matadd(Matadd(temp_1, temp_2, 2, 1), temp_3, 2, 1);
    double eigendenominator = i + j + k;

    G[0][0] = eigenvalues[0][0] / eigendenominator;
    G[1][0] = eigenvalues[1][0] / eigendenominator;                                 

   // printf("Incenter of Triangle IJK :%lf, %lf\n\n", G[0][0], G[1][0]);

sendHTMLForm(client_fd, sideIJ, sideJK, sideKI,L[0][0], L[1][0],midpoint_IJ[0][0],midpoint_IJ[1][0], midpoint_JK[0][0], midpoint_JK[1][0], midpoint_KI[0][0], midpoint_KI[1][0],Aa[0][0], Aa[1][0], Ba[0][0], Ba[1][0], Ca[0][0], Ca[1][0], G[0][0], G[1][0], k, i, j);
            printf("Results sent to client\n");

            freeMat(I,2);
            freeMat(J,2);
            freeMat(K,2);
            freeMat(m1,2);
            freeMat(m2,2);
            freeMat(m3,2);
            freeMat(L,2);
        }
        
        close(client_fd);
    }

    return 0;
}
