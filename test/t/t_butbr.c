#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_butbr *butbr;
    sp_noise *ns;
} UserData;

int t_butbr(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT noise = 0, butbr = 0;
    UserData ud;

    srand(1234567);

    sp_butbr_create(&ud.butbr);
    sp_noise_create(&ud.ns);

    sp_butbr_init(sp, ud.butbr);
    sp_noise_init(sp, ud.ns);

    for(n = 0; n < tst->size; n++) {
        noise = 0, butbr = 0;
        sp_noise_compute(sp, ud.ns, NULL, &noise);
        sp_butbr_compute(sp, ud.butbr, &noise, &butbr);
        sp_test_add_sample(tst, butbr);
    }

    if(sp_test_compare(tst, hash) == SP_NOT_OK) {
        printf("Generated hash %s does not match reference hash %s\n", 
                tst->md5string, hash);
        fail = 1;
    }
    
    sp_butbr_destroy(&ud.butbr);
    sp_noise_destroy(&ud.ns);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}