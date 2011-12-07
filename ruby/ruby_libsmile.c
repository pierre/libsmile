#include "ruby.h"

#include "smile.h"

static VALUE rb_mSmile;
static VALUE rb_cParser;

static VALUE libsmile_init()
{
    smile_decode_block_init();
    return Qnil;
}

#define BUFFER_SIZE 65536

static VALUE libsmile_decode(VALUE self, VALUE input)
{
    char result[BUFFER_SIZE] = {'\0'};

    Check_Type(input, T_STRING);

    // Reset for successive calls
    smile_decode_block_reset();

    smile_decode_block(result, BUFFER_SIZE, RSTRING_PTR(input), RSTRING_LEN(input));

    return rb_str_new2(result);
}

void Init_libsmile()
{
  rb_mSmile = rb_define_module("Smile");
  rb_cParser = rb_define_class_under(rb_mSmile, "Parser", rb_cObject);

  rb_define_method(rb_cParser, "initialize", libsmile_init, 0);
  rb_define_method(rb_cParser, "decode", libsmile_decode, 1);
}
