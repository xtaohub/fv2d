/*
 * fin:        main.cc
 * Author:      Xin Tao <xtao@ustc.edu.cn>
 *              Peng Peng <pp140594@mail.ustc.edu.cn>
 * Date:        05/12/2024 
 * 
 * Copyright (c) Xin Tao 
 *
 */

#include <iostream>
#include <cassert>
#include "Parameters.h"
#include "Mesh.h"
#include "D.h"
#include "BCs.h"
#include "Solver.h"
#include <ctime>

int main(int argc, char** argv) {

  Parameters paras(argc,argv); 

  // Create mesh 
  Mesh m(paras);

  // Create diffusion coefficients object
  D diffusion(paras, m);

  BCs boundary(paras);

  Solver solver(paras, m, diffusion, boundary);

  string filename;
  ofstream out; 

  // output coordinates 
  filename = paras.output_path() + "/" + paras.run_id() + "_x.dat";
  out.open(filename); 
  assert(out);
  out << m.x() << std::endl;  
  out.close();

  filename = paras.output_path() + "/" + paras.run_id() + "_y.dat";
  out.open(filename); 
  assert(out);
  out << m.y() << std::endl;  
  out.close();

  // The timer
  clock_t start, end;
  double cpu_time;
  start = clock();

  // Time loop for solving
  for (int k = 1; k <= paras.nsteps(); ++k) {

    // Solve using FVM solver
    solver.update();

    if(k % paras.save_every_step() == 0){
      filename = paras.output_path() + "/" + paras.run_id() + std::to_string(int((k) / paras.save_every_step()));
      out.open(filename);
      out << solver.f(); 
      out.close();
    }
  }
  end = clock();
  cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
  std::cout << "CPU time used " << cpu_time << " seconds" << std::endl;

  return 0;
}

