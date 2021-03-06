/**
 * Here some universal Vector functions
 *
 * Author(s):
 *   Michael Hufschmidt <michael@hufschmidt-web.de>
 */

#include <math.h>
#include "vector.h"

/// Gibt einen Vektor z = x + y zurück
Vector vector_add(const Vector x, const Vector y)
{
  Vector z = {{0, 0, 0}};
  z.x[0] = x.x[0] + y.x[0];
  z.x[1] = x.x[1] + y.x[1];
  z.x[2] = x.x[2] + y.x[2];
  return z;
}

/// Gibt einen Vektor z = x - y zurück
Vector vector_substract(const Vector x, const Vector y)
{
  Vector z = {{0, 0, 0}};
  z.x[0] = x.x[0] - y.x[0];
  z.x[1] = x.x[1] - y.x[1];
  z.x[2] = x.x[2] - y.x[2];
  return z;
}

/// Gibt einen Vektor z = a mal x zurück
Vector vector_scalar_mult(const Vector x, const double a)
{
  Vector z;
  z.x[0] = a * x.x[0];
  z.x[1] = a * x.x[1];
  z.x[2] = a * x.x[2];
  return z;
}

/// Gibt das Skalarprodukt p = x mal y zurück.
double vector_scalar_product(const Vector x, const Vector y)
{
  double p = 0;
  p = x.x[0] * y.x[0] + x.x[1] * y.x[1] + x.x[2] * y.x[2];
  return p;
}

/// Gibt den um den Winkel phi um die z-Achse rotierten Vektor x zurück
Vector vector_rotate_z(const Vector x, const double phi)
{
  Vector z = {{0, 0, 0}};
  z.x[0] = cos(phi) * x.x[0] - sin(phi) * x.x[1];
  z.x[1] = sin(phi) * x.x[0] + cos(phi) * x.x[1];
  z.x[2] = x.x[2];
  return z;
}

/// Gibt das Produkt einer 3x3 Matrix A mit einem Spaltenvektor x zurück
Vector matrix_times_vector(const Matrix A, const Vector x)
{
  int i, j;
  Vector z = {{0, 0, 0}};
  for (i = 0; i <= 2; i++)
  {
    z.x[i] = 0;
    for (j = 0; j <= 2; j++)
    {
      z.x[i] += A.m[i][j] * x.x[j];
    }
  }
  return z;
 }

/// Gibt die zu A transponierte Matrix B zurück
Matrix matrix_transpose(const Matrix A)
{
  int i, j;
  Matrix B;
  for (i = 0; i <= 2; i++)
  {
    for (j = 0; j <= 2; j++)
    {
      B.m[i][j] = A.m[j][i];
    }
  }
  return B;
}

/// Erzeugt eine Drehmatrix in der "x-Konvention"
/// aus den 3 Eulerschen Winkeln e1, e2, e3
/// Siehe http://de.wikipedia.org/wiki/Eulersche_Winkel
/// e1 = Phi, e2 = Theta, e3 = Psi
Matrix euler_rotate(const double e1, const double e2, const double e3)
{
  Matrix R = {{{1.0, 0, 0},{0, 1.0, 0},{0, 0, 1.0}}}; // Einheitsmatrix
  R.m[0][0] = 0.0 + cos(e3) * cos(e1) - cos(e2) * sin(e1) * sin(e3);
  R.m[0][1] = 0.0 + cos(e3) * sin(e1) + cos(e2) * cos(e1) * sin(e3);
  R.m[0][2] = 0.0 + sin(e3) * sin(e2);
  R.m[1][0] = 0.0 - sin(e3) * cos(e1) - cos(e2) * sin(e1) * cos(e3);
  R.m[1][1] = 0.0 - sin(e3) * sin(e1) + cos(e2) * cos(e1) * cos(e3);
  R.m[1][2] = 0.0 + cos(e3) * sin(e2);
  R.m[2][0] = 0.0 + sin(e2) * sin(e1);
  R.m[2][1] = 0.0 - sin(e2) * cos(e1);
  R.m[2][2] = 0.0 + cos(e2);
  return R;
}
