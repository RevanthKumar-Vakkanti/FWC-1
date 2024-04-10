#include <ArduinoOTA.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include"matfun.h"
#include"geofun.h"

AsyncWebServer server(80);

const char* ssid = "revi";
const char* password = "revi4922251";

const char* input_parameter00 = "input00";
const char* input_parameter01 = "input01";
const char* input_parameter10 = "input10";
const char* input_parameter11 = "input11";
const char* input_parameter20 = "input20";
const char* input_parameter21 = "input21";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>MATRIX_ALGORITHMS</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  
<style>
    body {font-family: Arial, sans-serif; background-color: #f0f0f0; text-align: center;}
    h2 {font-size: 2.5rem; color: #CA762B;}
    p {color: #666666;}
    form {margin-top: 20px;}
    input[type='number'] {width: 50px;}
    input[type='submit'] {background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer;}
    input[type='submit']:hover {background-color: #2B1B17;}
    #results {margin-top: 20px;}
  </style>
 
  <script>
    function updateResults(results) {
      document.getElementById("results").innerHTML = results;
    }
  </script>
  
  </head><body>
  <h2>MATRIX_ALGORITHMS_CALCULATION</h2>
  <h3>Enter the values of points A, B, C</h3>
  <form id="triangleForm" onsubmit="submitForm(); return false;">
              Enter the Point A's values here : <br> p1= <input type="number" value = "1" name="input00">
                                                      q2= <input type="number" value = "-1" name="input01"><br>
              
               Enter the Point B's values here : <br> p2= <input type="number" value = "-4" name="input10">
                                                      q2= <input type="number" value = "6" name="input11"><br>
              
               Enter the Point C's values here : <br> p3= <input type="number" value = "-3" name="input20">
                                                      q3= <input type="number" value = "-5" name="input21"><br>
              
    <input type="submit" value="Submit">
  </form><br>
  <div id="results"></div>
  
  <script>
    function submitForm() {
      var formData = new FormData(document.getElementById("triangleForm"));
      fetch('/get', { method: 'POST', body: formData })
        .then(response => response.text())
        .then(results => updateResults(results));
    }
  </script>
  
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connecting...");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_POST, [] (AsyncWebServerRequest *request) {
    double Ap = request->arg(input_parameter00).toDouble();
    double Aq= request->arg(input_parameter01).toDouble();
    double Bp = request->arg(input_parameter10).toDouble();
    double Bq = request->arg(input_parameter11).toDouble();
    double Cp = request->arg(input_parameter20).toDouble();
    double Cq = request->arg(input_parameter21).toDouble();


    
    int p1,q1,p2,q2,p3,q3,row=2,col=1,i,j,k=0;
    double **A,**B,**C,**m1,**m2,**m3,**L,**l,**N,**m3_t,**m1_t,**R0,**R1,**R,**R2;
    double **_ab, **_bc, **_ca, **bisectorABMidpoint, **bisectorBCMidpoint, **bisectorCAMidpoint;//Perpedicular bisector  
    double a, b, c;//Angular Bisector
    
    A = createMat(row,col);
    B = createMat(row,col);
    C = createMat(row,col);
    R = createMat(2,1);
    l = createMat(row,row);
    L = createMat(2,1);

    A[0][0] = Ap;
    A[1][0] = Aq;
    B[0][0] = Bp;
    B[1][0] = Bq;
    C[0][0] = Cp;
    C[1][0] = Cq;
    
    
    //Medinas
  double **D = Matsec(B, C, 2, 1.0);
  double **E = Matsec(C, A, 2, 1.0);
  double **F = Matsec(A, B, 2, 1.0);           
  double **s_ab, **s_bc, **s_ca;
  s_ab = Matsub(A,D,row,col);//A-D
  s_bc = Matsub(B,E,row,col);//B-E
  s_ca = Matsub(C,F,row,col);//C-F
  double sideAB = Matnorm(s_ab,row);
  double sideBC = Matnorm(s_bc,row); 
  double sideCA = Matnorm(s_ca,row);
  
//Orthocenter
    m1=Matsub(A,B,row,col);
    m2=Matsub(B,C,row,col);
    m3=Matsub(C,A,row,col);
    N=transposeMat(combineMat(m3,m1,row,col),row,row);
    m3_t=transposeMat(m3,row,col);
    m1_t=transposeMat(m1,row,col);
    R0=Matmul(m3_t,B,col,row,col);
    R1=Matmul(m1_t,C,col,row,col);
    R[0][0]=R0[0][0];
    R[1][0]=R1[0][0];
    
// Solving the linear equations
    L[1][0]=(-1)*(R[0][0]*N[1][0]-N[0][0]*R[1][0])/Matdet(N);
    L[0][0]=(R[0][0]*N[1][1]-N[0][1]*R[1][0])/Matdet(N);
  
//Perpedicular bisector      
  _ab = Matsub(A,B,row,col);//A-B
  _bc = Matsub(B,C,row,col);//B-C
  _ca = Matsub(C,A,row,col);//C-A
  bisectorABMidpoint = normVec(_ab);
  bisectorBCMidpoint = normVec(_bc);
  bisectorCAMidpoint = normVec(_ca);
  
//Angular Bisector
  a = Matnorm(Matsub(B, A, 2, 1), 2);  // Distance BC
  b = Matnorm(Matsub(C, A, 2, 1), 2);  // Distance AC
  c = Matnorm(Matsub(B, C, 2, 1), 2);  // Distance AB
  double l1 = (a + c - b) / 2;
  double l2 = (a + b - c) / 2;
  double l3 = (c + b - a) / 2;
  double x = (A[0][0] * l1 + B[0][0] * l2 + C[0][0] * l3) / (l1 + l2 + l3);
  double y = (A[1][0] * l1 + B[1][0] * l2 + C[1][0] * l3) / (l1 + l2 + l3);


  
// Send the results back to the client
    String result = "Results <br><br> Median AD: " + String(sideAB, 2) + "<br>Median BE: " + String(sideBC, 2) + "<br>Median CF: " + String(sideCA, 2) + "<br><br>";
    result += "Orthocenter O: (" + String(L[0][0],2) + ", " + String(L[1][0], 2) + ")<br><br>";
    result += "Bisector AB: (" + String(bisectorABMidpoint[0][0], 2) +", "+ String(bisectorABMidpoint[1][0],2) + ")<br>Bisector BC: (" + String(bisectorBCMidpoint[0][0], 2) +", "+ String(bisectorBCMidpoint[0][0], 2)+ ")<br>Bisector CA: (" + String(bisectorCAMidpoint[0][0], 2) +", "+ String(bisectorCAMidpoint[0][0],2) + ")<br><br>";
    result += "InCenter:(" + String(x, 2) + ", " + String(y, 2) + ")<br>";

    
    request->send(200, "text/html", result);
    freeMat(A,2);
    freeMat(B,2);
    freeMat(C,2);
    freeMat(m1,2);
    freeMat(m2,2);
    freeMat(m3,2);
    freeMat(L,2);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Nothing to do here
}
