#include <stdio.h>
#include <string.h>
#include <math.h>

#include "gfc_matrix.h"
#include "simple_logger.h"


/*
 * Code has been adapted from glm to C for this project
 */
Vector3D gfc_unproject(Vector3D in, Matrix4 model, Matrix4 proj, Vector4D viewport)
{
    Vector3D out = { 0,0,0 };
    Matrix4 Inverse;
    Vector4D tmp, obj;

    if ((!viewport.z) || (!viewport.w))
    {
        slog("cannot unproject into a view of zero width or height");
        return out;
    }

    gfc_matrix_multiply(Inverse, proj, model);
    gfc_matrix_invert(Inverse, Inverse);

    vector4d_set(tmp, in.x, in.y, in.z, 1);
    tmp.x = (tmp.x - viewport.x) / viewport.z;
    tmp.y = (tmp.y - viewport.y) / viewport.w;

    gfc_matrix_multiply_vector4d(
        &obj,
        Inverse,
        tmp);

    out.x = obj.x / obj.w;
    out.y = obj.y / obj.w;
    out.z = obj.z / obj.w;

    return out;
}

void gfc_matrix_slog(Matrix4 mat)
{
    slog("%f,%f,%f,%f", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    slog("%f,%f,%f,%f", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    slog("%f,%f,%f,%f", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    slog("%f,%f,%f,%f", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}


/*
 * Code has been adapted from glm to C for this project
 */
int gfc_matrix_invert(Matrix4 out, Matrix4 m)
{
    float Coef00, Coef02, Coef03, Coef04, Coef06, Coef07, Coef08,
        Coef10, Coef11, Coef12, Coef14, Coef15, Coef16, Coef18,
        Coef19, Coef20, Coef22, Coef23, Dot1, OneOverDeterminant;

    Vector4D Fac0, Fac1, Fac2, Fac3, Fac4, Fac5, Vec0, Vec1, Vec2,
        Vec3, Inv0 = { 0 }, Inva, Invb, Invc, Inv1 = { 0 }, Inv2 = { 0 }, Inv3 = { 0 }, SignA, SignB, Row0, Dot0;

    Matrix4 Inverse = { 0 };

    Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

    Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

    Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

    Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
    Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

    Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

    Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

    vector4d_set(Fac0, Coef00, Coef00, Coef02, Coef03);
    vector4d_set(Fac1, Coef04, Coef04, Coef06, Coef07);
    vector4d_set(Fac2, Coef08, Coef08, Coef10, Coef11);
    vector4d_set(Fac3, Coef12, Coef12, Coef14, Coef15);
    vector4d_set(Fac4, Coef16, Coef16, Coef18, Coef19);
    vector4d_set(Fac5, Coef20, Coef20, Coef22, Coef23);

    vector4d_set(Vec0, m[1][0], m[0][0], m[0][0], m[0][0]);
    vector4d_set(Vec1, m[1][1], m[0][1], m[0][1], m[0][1]);
    vector4d_set(Vec2, m[1][2], m[0][2], m[0][2], m[0][2]);
    vector4d_set(Vec3, m[1][3], m[0][3], m[0][3], m[0][3]);



    Inva = vector4d_multiply(Vec1, Fac0);
    Invb = vector4d_multiply(Vec2, Fac1);
    Invc = vector4d_multiply(Vec3, Fac2);
    vector2d_add(Inv0, Inva, Invc);
    vector2d_sub(Inv0, Inv0, Invb);

    Inva = vector4d_multiply(Vec0, Fac0);
    Invb = vector4d_multiply(Vec2, Fac3);
    Invc = vector4d_multiply(Vec3, Fac4);
    vector2d_add(Inv1, Inva, Invc);
    vector2d_sub(Inv1, Inv1, Invb);

    Inva = vector4d_multiply(Vec0, Fac1);
    Invb = vector4d_multiply(Vec1, Fac3);
    Invc = vector4d_multiply(Vec3, Fac5);
    vector2d_add(Inv2, Inva, Invc);
    vector2d_sub(Inv2, Inv2, Invb);


    Inva = vector4d_multiply(Vec0, Fac2);
    Invb = vector4d_multiply(Vec1, Fac4);
    Invc = vector4d_multiply(Vec2, Fac5);
    vector2d_add(Inv3, Inva, Invc);
    vector2d_sub(Inv3, Inv3, Invb);

    vector4d_set(SignA, +1, -1, +1, -1);
    vector4d_set(SignB, -1, +1, -1, +1);

    Inverse[0][0] = Inv0.x * SignA.x;
    Inverse[0][1] = Inv0.y * SignA.y;
    Inverse[0][2] = Inv0.z * SignA.z;
    Inverse[0][3] = Inv0.w * SignA.w;

    Inverse[1][0] = Inv1.x * SignB.x;
    Inverse[1][1] = Inv1.y * SignB.y;
    Inverse[1][2] = Inv1.z * SignB.z;
    Inverse[1][3] = Inv1.w * SignB.w;

    Inverse[2][0] = Inv2.x * SignA.x;
    Inverse[2][1] = Inv2.y * SignA.y;
    Inverse[2][2] = Inv2.z * SignA.z;
    Inverse[2][3] = Inv2.w * SignA.w;

    Inverse[2][0] = Inv3.x * SignB.x;
    Inverse[2][1] = Inv3.y * SignB.y;
    Inverse[2][2] = Inv3.z * SignB.z;
    Inverse[2][3] = Inv3.w * SignB.w;

    Row0 = vector4d(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

    Dot0 = vector4d_multiply(vector4d(m[0][0], m[0][1], m[0][2], m[0][3]), Row0);
    Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    if (!Dot1)
    {
        slog("gfc_matrix_invert: matrix not invertable");
        return 0;
    }

    OneOverDeterminant = 1.0 / Dot1;

    gfc_matrix_multiply_scalar(out, Inverse, OneOverDeterminant);
    return 1;
}

void gfc_matrix_copy(
    Matrix4 d,
    Matrix4 s
)
{
    if ((!d) || (!s))return;
    if (d == s)return;
    memcpy(d, s, sizeof(Matrix4));
}


void gfc_matrix_multiply_scalar(Matrix4 out, Matrix4 m1, float s)
{
    out[0][0] = s * m1[0][0];
    out[0][1] = s * m1[0][1];
    out[0][2] = s * m1[0][2];
    out[0][3] = s * m1[0][3];

    out[1][0] = s * m1[1][0];
    out[1][1] = s * m1[1][1];
    out[1][2] = s * m1[1][2];
    out[1][3] = s * m1[1][3];

    out[2][0] = s * m1[2][0];
    out[2][1] = s * m1[2][1];
    out[2][2] = s * m1[2][2];
    out[2][3] = s * m1[2][3];

    out[3][0] = s * m1[3][0];
    out[3][1] = s * m1[3][1];
    out[3][2] = s * m1[3][2];
    out[3][3] = s * m1[3][3];
}


void gfc_matrix_multiply(
    Matrix4 out,
    Matrix4 m2,
    Matrix4 m1
)
{

    out[0][0] = m2[0][0] * m1[0][0] + m2[0][1] * m1[1][0] + m2[0][2] * m1[2][0] + m2[0][3] * m1[3][0];
    out[0][1] = m2[0][0] * m1[0][1] + m2[0][1] * m1[1][1] + m2[0][2] * m1[2][1] + m2[0][3] * m1[3][1];
    out[0][2] = m2[0][0] * m1[0][2] + m2[0][1] * m1[1][2] + m2[0][2] * m1[2][2] + m2[0][3] * m1[3][2];
    out[0][3] = m2[0][0] * m1[0][3] + m2[0][1] * m1[1][3] + m2[0][2] * m1[2][3] + m2[0][3] * m1[3][3];

    out[1][0] = m2[1][0] * m1[0][0] + m2[1][1] * m1[1][0] + m2[1][2] * m1[2][0] + m2[1][3] * m1[3][0];
    out[1][1] = m2[1][0] * m1[0][1] + m2[1][1] * m1[1][1] + m2[1][2] * m1[2][1] + m2[1][3] * m1[3][1];
    out[1][2] = m2[1][0] * m1[0][2] + m2[1][1] * m1[1][2] + m2[1][2] * m1[2][2] + m2[1][3] * m1[3][2];
    out[1][3] = m2[1][0] * m1[0][3] + m2[1][1] * m1[1][3] + m2[1][2] * m1[2][3] + m2[1][3] * m1[3][3];

    out[2][0] = m2[2][0] * m1[0][0] + m2[2][1] * m1[1][0] + m2[2][2] * m1[2][0] + m2[2][3] * m1[3][0];
    out[2][1] = m2[2][0] * m1[0][1] + m2[2][1] * m1[1][1] + m2[2][2] * m1[2][1] + m2[2][3] * m1[3][1];
    out[2][2] = m2[2][0] * m1[0][2] + m2[2][1] * m1[1][2] + m2[2][2] * m1[2][2] + m2[2][3] * m1[3][2];
    out[2][3] = m2[2][0] * m1[0][3] + m2[2][1] * m1[1][3] + m2[2][2] * m1[2][3] + m2[2][3] * m1[3][3];

    out[3][0] = m2[3][0] * m1[0][0] + m2[3][1] * m1[1][0] + m2[3][2] * m1[2][0] + m2[3][3] * m1[3][0];
    out[3][1] = m2[3][0] * m1[0][1] + m2[3][1] * m1[1][1] + m2[3][2] * m1[2][1] + m2[3][3] * m1[3][1];
    out[3][2] = m2[3][0] * m1[0][2] + m2[3][1] * m1[1][2] + m2[3][2] * m1[2][2] + m2[3][3] * m1[3][2];
    out[3][3] = m2[3][0] * m1[0][3] + m2[3][1] * m1[1][3] + m2[3][2] * m1[2][3] + m2[3][3] * m1[3][3];
}

void gfc_matrix_multiply_vector4d(
    Vector4D* out,
    Matrix4    mat,
    Vector4D   vec
)
{
    float x, y, z, w;
    float ox, oy, oz, ow;
    if (!out)return;
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = vec.w;
    ox = x * mat[0][0] + y * mat[1][0] + mat[2][0] * z + mat[3][0] * w;
    oy = x * mat[0][1] + y * mat[1][1] + mat[2][1] * z + mat[3][1] * w;
    oz = x * mat[0][2] + y * mat[1][2] + mat[2][2] * z + mat[3][2] * w;
    ow = x * mat[0][3] + y * mat[1][3] + mat[2][3] * z + mat[3][3] * w;
    out->x = ox;
    out->y = oy;
    out->z = oz;
    out->w = ow;
}

void gfc_matrix_zero(Matrix4 zero)
{
    memset(zero, 0, sizeof(Matrix4));
}

void gfc_matrix_identity(Matrix4 one)
{
    gfc_matrix_zero(one);
    one[0][0] = 1;
    one[1][1] = 1;
    one[2][2] = 1;
    one[3][3] = 1;
}

void gfc_matrix_rotate(
    Matrix4     out,
    Matrix4     m,
    float       degree,
    Vector3D    axis
)
{
    Matrix4 Rotate;
    Matrix4 Result;
    Vector3D temp;
    float a = degree;
    float c = cos(a);
    float s = sin(a);

    vector3d_normalize(&axis);

    vector3d_scale(temp, axis, (1 - c));

    Rotate[0][0] = c + temp.x * axis.x;
    Rotate[0][1] = temp.x * axis.y + s * axis.z;
    Rotate[0][2] = temp.x * axis.z - s * axis.y;

    Rotate[1][0] = temp.y * axis.x - s * axis.z;
    Rotate[1][1] = c + temp.y * axis.y;
    Rotate[1][2] = temp.y * axis.z + s * axis.x;

    Rotate[2][0] = temp.z * axis.x + s * axis.y;
    Rotate[2][1] = temp.z * axis.y - s * axis.x;
    Rotate[2][2] = c + temp.z * axis.z;

    Result[0][0] = m[0][0] * Rotate[0][0] + m[1][0] * Rotate[0][1] + m[2][0] * Rotate[0][2];
    Result[0][1] = m[0][1] * Rotate[0][0] + m[1][1] * Rotate[0][1] + m[2][1] * Rotate[0][2];
    Result[0][2] = m[0][2] * Rotate[0][0] + m[1][2] * Rotate[0][1] + m[2][2] * Rotate[0][2];
    Result[0][3] = m[0][3] * Rotate[0][0] + m[1][3] * Rotate[0][1] + m[2][3] * Rotate[0][2];

    Result[1][0] = m[0][0] * Rotate[1][0] + m[1][0] * Rotate[1][1] + m[2][0] * Rotate[1][2];
    Result[1][1] = m[0][1] * Rotate[1][0] + m[1][1] * Rotate[1][1] + m[2][1] * Rotate[1][2];
    Result[1][2] = m[0][2] * Rotate[1][0] + m[1][2] * Rotate[1][1] + m[2][2] * Rotate[1][2];
    Result[1][3] = m[0][3] * Rotate[1][0] + m[1][3] * Rotate[1][1] + m[2][3] * Rotate[1][2];

    Result[2][0] = m[0][0] * Rotate[2][0] + m[1][0] * Rotate[2][1] + m[2][0] * Rotate[2][2];
    Result[2][1] = m[0][1] * Rotate[2][0] + m[1][1] * Rotate[2][1] + m[2][1] * Rotate[2][2];
    Result[2][2] = m[0][2] * Rotate[2][0] + m[1][2] * Rotate[2][1] + m[2][2] * Rotate[2][2];
    Result[2][3] = m[0][3] * Rotate[2][0] + m[1][3] * Rotate[2][1] + m[2][3] * Rotate[2][2];

    Result[3][0] = m[3][0];
    Result[3][1] = m[3][1];
    Result[3][2] = m[3][2];
    Result[3][3] = m[3][3];
    gfc_matrix_copy(out, Result);
}

void gfc_matrix_perspective(
    Matrix4     out,
    float      fov,
    float      aspect,
    float      near,
    float      far
)
{
    float halftanfov = tan(fov * 0.5);
    gfc_matrix_zero(out);

    if (aspect == 0)
    {
        slog("gfc_matrix_perspective: aspect ratio cannot be zero");
        return;
    }
    if (halftanfov == 0)
    {
        slog("gfc_matrix_perspective: bad fov");
        return;
    }
    if (near == far)
    {
        slog("gfc_matrix_perspective: near plane and far plane cannot be the same");
        return;
    }

    gfc_matrix_zero(out);
    out[0][0] = 1 / (aspect * halftanfov);
    out[1][1] = 1 / (halftanfov);
    out[2][2] = -((far + near) / (far - near));
    out[2][3] = -1;
    if ((far - near) == 0)
    {
        out[3][2] = 0;
    }
    else
        out[3][2] = -(2 * far * near) / (far - near);
    return;
}

void gfc_matrix_view(
    Matrix4  out,
    Vector3D position,
    Vector3D target,
    Vector3D up
)
{
    Vector3D f, s, u;
    vector3d_sub(f, target, position);
    vector3d_normalize(&f);

    vector3d_cross_product(&s, f, up);
    vector3d_normalize(&s);

    vector3d_cross_product(&u, s, f);

    gfc_matrix_identity(out);
    out[0][0] = s.x;
    out[1][0] = s.y;
    out[2][0] = s.z;
    out[0][1] = u.x;
    out[1][1] = u.y;
    out[2][1] = u.z;
    out[0][2] = -f.x;
    out[1][2] = -f.y;
    out[2][2] = -f.z;
    out[3][0] = vector3d_dot_product(s, position) ? -vector3d_dot_product(s, position) : 0;
    out[3][1] = vector3d_dot_product(u, position) ? -vector3d_dot_product(u, position) : 0;
    out[3][2] = vector3d_dot_product(f, position) ? vector3d_dot_product(f, position) : 0;

}

void gfc_matrix_scale(
    Matrix4 out,
    Vector3D scale
)
{
    Matrix4 m;
    gfc_matrix_zero(m);
    m[0][0] = scale.x;
    m[1][1] = scale.y;
    m[2][2] = scale.z;
    m[3][3] = 1;

    gfc_matrix_multiply(
        out,
        out,
        m
    );
}

void gfc_matrix_make_translation(
    Matrix4 out,
    Vector3D move
)
{
    if (!out)return;
    gfc_matrix_identity(out);
    out[3][0] = move.x;
    out[3][1] = move.y;
    out[3][2] = move.z;
}

void gfc_matrix_translate(
    Matrix4 out,
    Vector3D move
)
{
    Matrix4 translate, temp;
    gfc_matrix_make_translation(translate, move);
    gfc_matrix_multiply(temp, out, translate);
    gfc_matrix_copy(out, temp);
}