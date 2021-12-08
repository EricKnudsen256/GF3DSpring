#ifndef __GFC_MATRIX_H__
#define __GFC_MATRIX_H__

#include "gfc_vector.h"

typedef float Matrix4[4][4];

typedef struct
{
    Matrix4 model;
    Matrix4 view;
    Matrix4 proj;
}UniformBufferObject;

/**
 * @brief copy the contents of one matrix into another
 * @param d the destination matrix
 * @param s the source matrix
 */
void gfc_matrix_copy(
    Matrix4 d,
    Matrix4 s
);

/**
 * @brief set the matrix to an identity matrix
 * @param one the matrix to become an identity
 */
void gfc_matrix_identity(Matrix4 one);

/**
 * @brief set the matrix to a zero matrix
 * @param zero the matrix to be set to zero
 */
void gfc_matrix_zero(Matrix4 zero);

/**
 * @brief invert a matrix
 * @param out the resulting inverse matrix
 * @param in the matrix to invert
 * @return 1 on success, 0 if the matrix has no inverse.  On 0, out will not be changed
 */
int gfc_matrix_invert(Matrix4 out, Matrix4 in);

/**
 * @brief create a translation matrix given the vector
 * @param out the output matrix, the contents of this matrix are overwritten
 * @param move the vector describing the translation
 */
void gfc_matrix_make_translation(
    Matrix4 out,
    Vector3D move
);

void gfc_matrix_translate(
    Matrix4 out,
    Vector3D move
);

/**
 * @brief setup a view matrix for a frustum centered at position, pointed at target, with up as the up direction
 * @note adapted from glm
 * @param out output matrix
 * @param position position of the "camera"
 * @param target location to look
 * @param up the direction considered "up"
 */
void gfc_matrix_view(
    Matrix4  out,
    Vector3D position,
    Vector3D target,
    Vector3D up
);

void gfc_matrix_slog(Matrix4 mat);

/**
 * @brief setup a perspective projection matrix
 * @note adapted from glm
 * @param out the output matrix
 * @param fov the field of view
 * @param aspect aspect ration (screen width / screen height)
 * @param near the near z plane
 * @param far the far z plane
 */
void gfc_matrix_perspective(
    Matrix4     out,
    float      fov,
    float      aspect,
    float      near,
    float      far
);

/**
 * @brief map screen coordinates into 3d world coordinate
 * @note use this twice, with z value 0 and z value 1 to create a trace segment to determine what
 *       is beneath the screen position
 * @param in the screen coordinates to map
 * @param modelview the view matrix of the scene
 * @param proj the projection matrix of the scene
 * @param viewport screen offset (x,y) and extent (z,w)
 * @return 3d scene coordinates of the screen coordinates
 */
Vector3D gfc_unproject(Vector3D in, Matrix4 modelview, Matrix4 proj, Vector4D viewport);

/**
 * @brief multiply the two input matrices together and save the result into out
 * @note this is not safe if out is one of the inputs
 * @note operation is out = a * b
 * @param out the output matrix
 * @param a one multiplicand matrix
 * @param b another multiplicand matrix
 */
void gfc_matrix_multiply(
    Matrix4 out,
    Matrix4 a,
    Matrix4 b
);

/**
 * @brief multiply a vector by the matrix, saving the result in an vector
 * @param out a pointer to the vector that will hold the result
 * @param mat input matrix to multiply by
 * @param vec input matrix to multiply by
 */
void gfc_matrix_multiply_vector4d(
    Vector4D* out,
    Matrix4    mat,
    Vector4D   vec
);

/**
 * @brief multiply a matrix by the scalar value
 * @param out the output matrix
 * @param m1 input matrix to multiply by
 * @param s input scalar value to multiply by
 */
void gfc_matrix_multiply_scalar(Matrix4 out, Matrix4 m1, float s);


/**
 * @brief multiply a matrix by the rotation matrix
 * @param out the output matrix
 * @param in  the input matrix
 * @param degree the amount, in radians, to rotate by
 * @param axis the axis about which to rotate
 */
void gfc_matrix_rotate(
    Matrix4     out,
    Matrix4     in,
    float       degree,
    Vector3D    axis
);

/**
 * @brief scale a matrix based on the input scale vector
 * @param out the matrix to be scaled.  it is changed by this function
 * @param scale the amount to scale it by
 */
void gfc_matrix_scale(
    Matrix4 out,
    Vector3D scale
);


#endif