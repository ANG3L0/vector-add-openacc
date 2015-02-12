#include <wb.h> 

void vecAdd(float *out, float *in1, float *in2, int N){
#pragma acc parallel num_gangs(256) copyin(in1[0:N]) copyin(in2[0:N]) copyout(out[0:N])
  {
#pragma acc loop gang
    for (int i = 0; i < N; i++){
      out[i] = in1[i] + in2[i];
    }
  }
}

int main(int argc, char **argv) {
  wbArg_t args;
  int inputLength;
  float *hostInput1;
  float *hostInput2;
  float *hostOutput;

  args = wbArg_read(argc, argv);

  wbTime_start(Generic, "Importing data and creating memory on host");
  hostInput1 = (float *)wbImport(wbArg_getInputFile(args, 0), &inputLength);
  hostInput2 = (float *)wbImport(wbArg_getInputFile(args, 1), &inputLength);
  hostOutput = (float *)malloc(inputLength * sizeof(float));
  wbTime_stop(Generic, "Importing data and creating memory on host");

  vecAdd(hostOutput, hostInput1, hostInput2, inputLength);

  wbLog(TRACE, "The input length is ", inputLength);


  wbSolution(args, hostOutput, inputLength);

  free(hostInput1);
  free(hostInput2);
  free(hostOutput);

  return 0;
}

