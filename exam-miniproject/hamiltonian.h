/**
 * @file hamiltonian.h
 *
 * @author unknown, possibly Elias Rudberg, a researcher at Uppsala
 * University, 2017 or earlier. This prelude, include guards,
 * descriptions and other comments added by Erik Vesterlund.
 *
 * @brief ...
 */

#ifndef HAMILTONIAN_H
#define HAMILTONIAN_H

/**
 * @brief Harmonic oscillator potential
 *
 * @param x
 * @param mass
 * @param omega
 * @return ...
 */
double get_potential_value(double x, double mass, double omega);

/**
 * @brief ...
 *
 * @param i
 * @param j
 * @param N
 * @param h
 * @param mass
 * @return ...
 */
double get_kinetic_energy_term(int i, int j, int N, double h, double mass);

/**
 * @brief ...
 *
 * @param i
 * @param j
 * @param N
 * @param h
 * @param mass
 * @param omega
 * @return ...
 */
double get_potential_energy_term(int i, int j, int N, double h, double mass, double omega);

/**
 * @brief ...
 *
 * @param i
 * @param j
 * @param N
 * @param h
 * @param mass
 * @param omega
 * @return ...
 */
double get_H_matrix_element(int i, int j, int N, double h, double mass, double omega);

#endif /* HAMILTONIAN_H */
