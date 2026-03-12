// 2026/1/31

#ifndef CLOSURE_H
#define CLOSURE_H

#define closure_meta(catch_list, return_type, name, param_list, body)               \
    struct _catch_list_##name catch_list;                                           \
    struct _param_list_##name param_list;                                           \
    struct _closure_type_##name {                                                   \
        struct _catch_list_##name catch;                                            \
    };                                                                              \
    return_type _closure_##name(                                                    \
        struct _catch_list_##name   catch,                                          \
        struct _param_list_##name   param                                           \
    ) {                                                                             \
        body;                                                                       \
    }

#define closure(catch_list, name)               \
    ((struct _closure_type_##name) {            \
        (struct _catch_list_##name)catch_list   \
    })

#define closure_t(meta) struct _closure_type_##meta

#define closure_call(meta, closure, param_list) \
    (_closure_##meta((closure).catch, (struct _param_list_##meta)param_list))

#endif
