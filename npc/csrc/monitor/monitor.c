#include <memory/paddr.h>

void init_log(const char *log_file);
void init_mem();
void init_isa();
void init_sdb();

static char *log_file = NULL;
static char *img_file = NULL;

static long load_img(){
    if (img_file == NULL){
        //Log("No image is given. Use the default build-in image.");
        return 4096; // built-in image size
    }
    FILE *fp = fopen(img_file, "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    //Log("The image is %s, size = %ld", img_file, size);

    fseek(fp, 0, SEEK_SET);
    int ret = fread(guest_to_host(CONFIG_MBASE), size, 1, fp);
    assert(ret == 1);

    fclose(fp);
    return size;
}

#include <getopt.h>

static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"log"      , required_argument, NULL, 'l'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-l:", table, NULL)) != -1) {
    switch (o) {
      case 'l': log_file = optarg; break;
      case 1: img_file = optarg; return 0;
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\n");
    }
  }
  return 0;
}

void init_monitor(int argc, char *argv[]){
    parse_args(argc, argv);
    init_log(log_file);
    init_mem();
    init_isa();
    long img_size = load_img();
    init_sdb();
}