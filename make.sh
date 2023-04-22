clear

rm -v a.out*

g++ -O3 -x c++ knupdown3.C -D KNH -o a.out_knh -D DEBUGLEVEL=0 -D DISABLE_ELEMENT_ORDER_ASSERTIONS

g++ -O3 -x c++ knupdown3.C -D KNH_BINDING -o a.out_knh_bindings -D DEBUGLEVEL=0 -D DISABLE_ELEMENT_ORDER_ASSERTIONS

g++ -O3 -x c++ knupdown3.C -D KNH_BINDING -D KNH_BINDING_IMPL=knheap_c_binding.c -o a.out_knh_bindings_vec -D DEBUGLEVEL=0 -D DISABLE_ELEMENT_ORDER_ASSERTIONS
