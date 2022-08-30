// Fonte: https://gitlab.com/fse_fga/trabalhos-2022_1/trabalho-2-2022-1/-/blob/main/PID/pid.h

#ifndef PID_HPP_
#define PID_HPP_

void pid_configura_constantes(double Kp_, double Ki_, double Kd_);
void pid_atualiza_referencia(float referencia_);
double pid_controle(double saida_medida);

#endif /* PID_HPP_ */