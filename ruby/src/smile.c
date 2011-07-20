#include "ruby.h"

#include "smile_decode.h"

static VALUE rb_mSmile;
static VALUE rb_cParser;

static FILE* file;

static void file_mark(FILE *f)
{
  /* Nothing needed */
}

static void file_free(FILE *f)
{
    if (f) {
        fclose(f);
    }
}

static VALUE file_alloc(VALUE klass)
{
    FILE *f = NULL;
    return Data_Wrap_Struct(klass, file_mark, file_free, f);
}
static VALUE file_init(VALUE obj, VALUE path)
{
    file = fopen(RSTRING(path)->ptr, "r");

    if (file == NULL) {
        rb_raise(rb_eRuntimeError, "Invalid file specified");
    }

    return Qnil;
}

static VALUE check_header(VALUE self)
{
    u8 header[4];

    int bytes_read = fread(header, sizeof(header), 4, file);

    if (bytes_read != 4) {
        rb_raise(rb_eRuntimeError, "Could not read header");
        return Qfalse;
    } else {
        if (smile_decode_header(header)) {
            return Qtrue;
        } else {
            return Qfalse;
        }
    }
}

void Init_smile()
{
  rb_mSmile = rb_define_module("Smile");
  rb_cParser = rb_define_class_under(rb_mSmile, "Parser", rb_cObject);
  rb_define_alloc_func(rb_cParser, file_alloc);
  rb_define_method(rb_cParser, "initialize", file_init, 1);

  rb_define_method(rb_cParser, "check_header", check_header, 0);
}
