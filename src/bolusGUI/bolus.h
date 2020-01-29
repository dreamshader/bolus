#ifndef BOLUS_H
#define BOLUS_H

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

struct _bolus_param {
    bool fail;
    int  glucose;
    int  carb;
    int  bread;
    char mealType;
    char measType;
    int adjust;
    bool last;
    char editType;
    char exportType;
    char *importFile;
    bool interactive;
    bool noStore;
    bool offset;
    bool query;
};


#ifdef __cplusplus
}
#endif



#endif // BOLUS_H
