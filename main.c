/*2019 Arda Eden
  Parser for MVN files*/

#include "m_pd.h"
#include <string.h>

#define WORDSIZE 4
#define HEADERSIZE 24
#define SEGMENT_DATA_SIZE_EULER 28
#define SEGMENT_DATA_SIZE_QUARTERNION 32

static t_class *mvnparse_class;

typedef struct _mvnparse
{
    t_object x_obj;
    t_outlet *l_out;
} t_mvnparse;



static void mvnparse_list(t_mvnparse *x, t_symbol *s, int argc, t_atom *argv)
{
    uint8_t tmp_Id[4], tmp_X[4], tmp_Y[4], tmp_Z[4];
    uint32_t o_id;
    t_float o_x, o_y, o_z;
    t_atom outData[4];

    for (int j=0; j<23; j++) {

        for (int i=0; i<4; i++) {
            tmp_Id[i]=(uint8_t)atom_getfloat(argv+3+HEADERSIZE+(j*32)+0-i);
            tmp_X[i] =(uint8_t)atom_getfloat(argv+3+HEADERSIZE+(j*32)+4-i);
            tmp_Y[i] =(uint8_t)atom_getfloat(argv+3+HEADERSIZE+(j*32)+8-i);
            tmp_Z[i] =(uint8_t)atom_getfloat(argv+3+HEADERSIZE+(j*32)+12-i);
        }

        memcpy(&o_id, &tmp_Id, 4);
        memcpy(&o_x, &tmp_X, 4);
        memcpy(&o_y, &tmp_Y, 4);
        memcpy(&o_z, &tmp_Z, 4);

        SETFLOAT(outData, o_id);
        SETFLOAT(outData+1, o_x);
        SETFLOAT(outData+2, o_y);
        SETFLOAT(outData+3, o_z);

        outlet_list(x->l_out, 0, 4, outData);
    }
}

static t_mvnparse *mvnparse_new(t_symbol *s, int argc, t_atom *argv)
{
    t_mvnparse *x = (t_mvnparse *)pd_new(mvnparse_class);
    x->l_out=outlet_new(&x->x_obj, gensym("list"));
    return (x);
}

void mvnparse_setup(void)
{
    mvnparse_class = class_new(gensym("mvnparse"), (t_newmethod)mvnparse_new,
        0, sizeof(t_mvnparse), 0, A_GIMME, 0);
    class_addlist(mvnparse_class, mvnparse_list);
}
