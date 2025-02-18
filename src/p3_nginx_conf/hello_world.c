#include <fcgiapp.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  FCGX_Request req;
  FCGX_Init();
  FCGX_InitRequest(&req, 0, 0);

  while (FCGX_Accept_r(&req) >= 0) {
    FCGX_FPrintF(req.out, "Content-type: text/html\r\n"
                          "Content-Length: 12\r\n"
                          "\r\n"
                          "Hello World!");
  }

  return 0;
}
