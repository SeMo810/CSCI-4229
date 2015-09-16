#ifndef LORENZ_H_
#define LORENZ_H_

/* Gets the current Lorenz parameters. */
double lorenz_get_s_parameter(void);
double lorenz_get_b_parameter(void);
double lorenz_get_r_parameter(void);
void lorenz_get_parameters(double *s, double *b, double *r);

/* Set the current Lorenz parameters. */
void lorenz_set_s_parameter(double s);
void lorenz_set_b_parameter(double b);
void lorenz_set_r_parameter(double r);
void lorenz_set_parameters(double s, double b, double r);

#endif // LORENZ_H_
