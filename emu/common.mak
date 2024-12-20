OBJ = dis.o init.o init_sym.o loader.o inst_inf.o main.o toam.o unify.o \
	file.o domain.o cfd.o float1.o arith.o token.o global.o \
	builtins.o mic.o numbervars.o cpreds.o univ.o assert_bp.o findall.o clause.o \
    delay.o clpfd.o clpfd_libs.o event.o toamprofile.o \
    kapi.o getline.o table.o gcstack.o gcheap.o gcqueue.o debug.o \
    expand_bp.o bigint.o sapi.o kissat_picat.o espresso_bp.o \
	picat_utilities.o fann.o fann_cascade.o fann_error.o fann_io.o fann_train.o fann_train_data.o fann_interface.o bp_pcre2.o

ESPRESSO_FLAGS = -O3 -I. -Iespresso
ESPRESSO_OBJ = black_white.o canonical.o cofactor.o cols.o compl.o contain.o cpu_time.o cubestr.o \
               cvrin.o cvrm.o cvrmisc.o cvrout.o dominate.o equiv.o espresso.o espresso_expand.o \
               essen.o essentiality.o exact.o gasp.o gimpel.o globals.o hack.o indep.o irred.o \
               map.o matrix.o mincov.o opo.o pair.o part.o primes.o prtime.o reduce.o rows.o \
               set.o setc.o sharp.o sigma.o signature.o signature_exact.o sminterf.o solution.o \
               sparse.o unate.o util_signature.o verify.o

KISSAT_FLAGS = -O3 -W -Wall -DNEMBEDDED -DNDEBUG -DNMETRICS -DQUIET -DNSTATISTICS  -DNPROOFS
KISSAT_OBJ =  kis_allocate.o kis_analyze.o kis_ands.o kis_application.o\
              kis_arena.o kis_assign.o kis_autarky.o kis_averages.o kis_backtrack.o\
              kis_backward.o kis_build.o kis_bump.o kis_check.o kis_clause.o\
              kis_clueue.o kis_collect.o kis_colors.o kis_compact.o kis_config.o\
              kis_decide.o kis_deduce.o kis_dense.o kis_dominate.o kis_dump.o\
              kis_eliminate.o kis_equivalences.o kis_error.o kis_extend.o kis_failed.o\
              kis_file.o kis_flags.o kis_format.o kis_forward.o kis_frames.o\
              kis_gates.o kis_handle.o kis_heap.o kis_ifthenelse.o kis_import.o\
              kis_internal.o kis_learn.o kis_limits.o kis_logging.o kis_main.o\
              kis_minimize.o kis_mode.o kis_options.o kis_parse.o kis_phases.o\
              kis_print.o kis_probe.o kis_profile.o kis_promote.o kis_proof.o\
              kis_propdense.o kis_prophyper.o kis_proprobe.o kis_propsearch.o kis_queue.o\
              kis_reduce.o kis_reluctant.o kis_rephase.o kis_report.o kis_resize.o\
              kis_resolve.o kis_resources.o kis_restart.o kis_search.o kis_smooth.o\
              kis_sort.o kis_stack.o kis_statistics.o kis_strengthen.o kis_substitute.o\
              kis_terminate.o kis_ternary.o kis_trail.o kis_transitive.o kis_utilities.o\
              kis_vector.o kis_vivify.o kis_walk.o kis_watch.o kis_weaken.o\
              kis_witness.o kis_xors.o

picat$(EXT) : $(OBJ) $(ESPRESSO_OBJ) $(KISSAT_OBJ)
	$(CPP) -o picat$(EXT) $(CFLAGS) $(OBJ) $(ESPRESSO_OBJ)  $(KISSAT_OBJ) $(LFLAGS)
clean :
	rm -f *.o picat$(EXT)
dis.o   : dis.c term.h inst.h basic.h
	$(CCC) $(CFLAGS) dis.c
init.o  : init.c term.h inst.h basic.h
	$(CCC) $(CFLAGS) init.c
init_sym.o  : init_sym.c term.h inst.h basic.h
	$(CCC) $(CFLAGS) init_sym.c
loader.o : loader.c term.h basic.h inst.h picat_bc.h
	$(CCC) $(CFLAGS) loader.c
inst_inf.o  : inst_inf.c term.h inst.h basic.h
	$(CCC) $(CFLAGS) inst_inf.c
main.o  : main.c term.h inst.h basic.h
	$(CCC) $(CFLAGS) main.c
toam.o  : toam.c term.h inst.h basicd.h basic.h toam.h event.h frame.h emu_inst.h
	$(CCC) $(CFLAGS) toam.c
toamprofile.o  : toamprofile.c term.h inst.h basicd.h basic.h toam.h event.h frame.h
	$(CCC) $(CFLAGS) toamprofile.c
unify.o : unify.c term.h basic.h event.h bapi.h frame.h
	$(CCC) $(CFLAGS) unify.c
file.o	: file.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) file.c
domain.o	: domain.c term.h basic.h bapi.h event.h frame.h
	$(CCC) $(CFLAGS) domain.c
cfd.o	: cfd.c term.h basic.h bapi.h event.h frame.h
	$(CCC) $(CFLAGS) cfd.c
float1.o	: float1.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) float1.c
arith.o	: arith.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) arith.c
bigint.o	: bigint.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) bigint.c
token.o	: token.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) token.c
global.o	: global.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) global.c
builtins.o	: builtins.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) builtins.c
mic.o	: mic.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) mic.c
numbervars.o	: numbervars.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) numbervars.c
cpreds.o	: cpreds.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) cpreds.c
univ.o	: univ.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) univ.c
assert_bp.o	: assert_bp.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) assert_bp.c
findall.o	: findall.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) findall.c
clause.o	: clause.c term.h basic.h bapi.h dynamic.h
	$(CCC) $(CFLAGS) clause.c
delay.o	: delay.c term.h basic.h bapi.h
	$(CCC) $(CFLAGS) delay.c
clpfd.o : clpfd.c term.h basic.h bapi.h event.h frame.h
	$(CCC) $(CFLAGS) clpfd.c
clpfd_libs.o : clpfd_libs.c term.h basic.h bapi.h event.h frame.h
	$(CCC) $(CFLAGS) clpfd_libs.c
kapi.o : kapi.c term.h basic.h bapi.h kapi.h
	$(CCC) $(CFLAGS) kapi.c
ki.o : kapi.c term.h basic.h bapi.h kapi.h
	$(CCC) $(CFLAGS) kapi.c
event.o : event.c term.h basic.h bapi.h kapi.h event.h
	$(CCC) $(CFLAGS) event.c
table.o : table.c term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) table.c
debug.o : debug.c term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) debug.c
gcstack.o : gcstack.c gc.h term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) gcstack.c
gcheap.o : gcheap.c gc.h term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) gcheap.c
gQueue.o : gcqueue.c gc.h term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) gcqueue.c
expand_bp.o : expand_bp.c gc.h term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) expand_bp.c
espresso_bp.o : espresso_bp.c term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) -Iespresso espresso_bp.c
sapi.o : sapi.c sapi.h term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) sapi.c
kissat_picat.o : kissat_picat.c term.h basic.h bapi.h frame.h
	$(CCC) $(CFLAGS) kissat_picat.c
black_white.o : espresso/black_white.c
	$(CCC) $(ESPRESSO_FLAGS)  -o black_white.o espresso/black_white.c
canonical.o : espresso/canonical.c
	$(CCC) $(ESPRESSO_FLAGS) -o canonical.o espresso/canonical.c
cofactor.o : espresso/cofactor.c
	$(CCC) $(ESPRESSO_FLAGS) -o cofactor.o espresso/cofactor.c
cols.o : espresso/cols.c
	$(CCC) $(ESPRESSO_FLAGS) -o cols.o espresso/cols.c
compl.o : espresso/compl.c
	$(CCC) $(ESPRESSO_FLAGS) -o compl.o espresso/compl.c
contain.o : espresso/contain.c
	$(CCC) $(ESPRESSO_FLAGS) -o contain.o espresso/contain.c
cpu_time.o : espresso/cpu_time.c
	$(CCC) $(ESPRESSO_FLAGS) -o cpu_time.o espresso/cpu_time.c
cubestr.o : espresso/cubestr.c
	$(CCC) $(ESPRESSO_FLAGS) -o cubestr.o espresso/cubestr.c
cvrin.o : espresso/cvrin.c
	$(CCC) $(ESPRESSO_FLAGS) -o cvrin.o espresso/cvrin.c
cvrm.o : espresso/cvrm.c
	$(CCC) $(ESPRESSO_FLAGS) -o cvrm.o espresso/cvrm.c
cvrmisc.o : espresso/cvrmisc.c
	$(CCC) $(ESPRESSO_FLAGS) -o cvrmisc.o espresso/cvrmisc.c
cvrout.o : espresso/cvrout.c
	$(CCC) $(ESPRESSO_FLAGS) -o cvrout.o espresso/cvrout.c
dominate.o : espresso/dominate.c
	$(CCC) $(ESPRESSO_FLAGS) -o dominate.o espresso/dominate.c
equiv.o : espresso/equiv.c
	$(CCC) $(ESPRESSO_FLAGS) -o equiv.o espresso/equiv.c
espresso.o : espresso/espresso.c
	$(CCC) $(ESPRESSO_FLAGS) -o espresso.o espresso/espresso.c
espresso_expand.o : espresso/espresso_expand.c
	$(CCC) $(ESPRESSO_FLAGS) -o espresso_expand.o espresso/espresso_expand.c
espresso_main.o : espresso/espresso_main.c
	$(CCC) $(ESPRESSO_FLAGS) -o espresso_main.o espresso/espresso_main.c
essen.o : espresso/essen.c
	$(CCC) $(ESPRESSO_FLAGS) -o essen.o espresso/essen.c
essentiality.o : espresso/essentiality.c
	$(CCC) $(ESPRESSO_FLAGS) -o essentiality.o espresso/essentiality.c
exact.o : espresso/exact.c
	$(CCC) $(ESPRESSO_FLAGS) -o exact.o espresso/exact.c
gasp.o : espresso/gasp.c
	$(CCC) $(ESPRESSO_FLAGS) -o gasp.o espresso/gasp.c
gimpel.o : espresso/gimpel.c
	$(CCC) $(ESPRESSO_FLAGS) -o gimpel.o espresso/gimpel.c
globals.o : espresso/globals.c
	$(CCC) $(ESPRESSO_FLAGS) -o globals.o espresso/globals.c
hack.o : espresso/hack.c
	$(CCC) $(ESPRESSO_FLAGS) -o hack.o espresso/hack.c
indep.o : espresso/indep.c
	$(CCC) $(ESPRESSO_FLAGS) -o indep.o espresso/indep.c
irred.o : espresso/irred.c
	$(CCC) $(ESPRESSO_FLAGS) -o irred.o espresso/irred.c
map.o : espresso/map.c
	$(CCC) $(ESPRESSO_FLAGS) -o map.o espresso/map.c
matrix.o : espresso/matrix.c
	$(CCC) $(ESPRESSO_FLAGS) -o matrix.o espresso/matrix.c
mincov.o : espresso/mincov.c
	$(CCC) $(ESPRESSO_FLAGS) -o mincov.o espresso/mincov.c
opo.o : espresso/opo.c
	$(CCC) $(ESPRESSO_FLAGS) -o opo.o espresso/opo.c
pair.o : espresso/pair.c
	$(CCC) $(ESPRESSO_FLAGS) -o pair.o espresso/pair.c
part.o : espresso/part.c
	$(CCC) $(ESPRESSO_FLAGS) -o part.o espresso/part.c
primes.o : espresso/primes.c
	$(CCC) $(ESPRESSO_FLAGS) -o primes.o espresso/primes.c
prtime.o : espresso/prtime.c
	$(CCC) $(ESPRESSO_FLAGS) -o prtime.o espresso/prtime.c
reduce.o : espresso/reduce.c
	$(CCC) $(ESPRESSO_FLAGS) -o reduce.o espresso/reduce.c
rows.o : espresso/rows.c
	$(CCC) $(ESPRESSO_FLAGS) -o rows.o espresso/rows.c
set.o : espresso/set.c
	$(CCC) $(ESPRESSO_FLAGS) -o set.o espresso/set.c
setc.o : espresso/setc.c
	$(CCC) $(ESPRESSO_FLAGS) -o setc.o espresso/setc.c
sharp.o : espresso/sharp.c
	$(CCC) $(ESPRESSO_FLAGS) -o sharp.o espresso/sharp.c
sigma.o : espresso/sigma.c
	$(CCC) $(ESPRESSO_FLAGS) -o sigma.o espresso/sigma.c
signature.o : espresso/signature.c
	$(CCC) $(ESPRESSO_FLAGS) -o signature.o espresso/signature.c
signature_exact.o : espresso/signature_exact.c
	$(CCC) $(ESPRESSO_FLAGS) -o signature_exact.o espresso/signature_exact.c
sminterf.o : espresso/sminterf.c
	$(CCC) $(ESPRESSO_FLAGS) -o sminterf.o espresso/sminterf.c
solution.o : espresso/solution.c
	$(CCC) $(ESPRESSO_FLAGS) -o solution.o espresso/solution.c
sparse.o : espresso/sparse.c
	$(CCC) $(ESPRESSO_FLAGS) -o sparse.o espresso/sparse.c
unate.o : espresso/unate.c
	$(CCC) $(ESPRESSO_FLAGS) -o unate.o espresso/unate.c
util_signature.o : espresso/util_signature.c
	$(CCC) $(ESPRESSO_FLAGS) -o util_signature.o espresso/util_signature.c
verify.o : espresso/verify.c
	$(CCC) $(ESPRESSO_FLAGS) -o verify.o espresso/verify.c
kis_allocate.o : kissat/src/allocate.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_allocate.o kissat/src/allocate.c
kis_analyze.o : kissat/src/analyze.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_analyze.o kissat/src/analyze.c
kis_ands.o : kissat/src/ands.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_ands.o kissat/src/ands.c
kis_application.o : kissat/src/application.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_application.o kissat/src/application.c
kis_arena.o : kissat/src/arena.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_arena.o kissat/src/arena.c
kis_assign.o : kissat/src/assign.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_assign.o kissat/src/assign.c
kis_autarky.o : kissat/src/autarky.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_autarky.o kissat/src/autarky.c
kis_averages.o : kissat/src/averages.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_averages.o kissat/src/averages.c
kis_backtrack.o : kissat/src/backtrack.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_backtrack.o kissat/src/backtrack.c
kis_backward.o : kissat/src/backward.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_backward.o kissat/src/backward.c
kis_build.o : kissat/src/build.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_build.o kissat/src/build.c
kis_bump.o : kissat/src/bump.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_bump.o kissat/src/bump.c
kis_check.o : kissat/src/check.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_check.o kissat/src/check.c
kis_clause.o : kissat/src/clause.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_clause.o kissat/src/clause.c
kis_clueue.o : kissat/src/clueue.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_clueue.o kissat/src/clueue.c
kis_collect.o : kissat/src/collect.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_collect.o kissat/src/collect.c
kis_colors.o : kissat/src/colors.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_colors.o kissat/src/colors.c
kis_compact.o : kissat/src/compact.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_compact.o kissat/src/compact.c
kis_config.o : kissat/src/config.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_config.o kissat/src/config.c
kis_decide.o : kissat/src/decide.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_decide.o kissat/src/decide.c
kis_deduce.o : kissat/src/deduce.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_deduce.o kissat/src/deduce.c
kis_dense.o : kissat/src/dense.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_dense.o kissat/src/dense.c
kis_dominate.o : kissat/src/dominate.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_dominate.o kissat/src/dominate.c
kis_dump.o : kissat/src/dump.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_dump.o kissat/src/dump.c
kis_eliminate.o : kissat/src/eliminate.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_eliminate.o kissat/src/eliminate.c
kis_equivalences.o : kissat/src/equivalences.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_equivalences.o kissat/src/equivalences.c
kis_error.o : kissat/src/error.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_error.o kissat/src/error.c
kis_extend.o : kissat/src/extend.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_extend.o kissat/src/extend.c
kis_failed.o : kissat/src/failed.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_failed.o kissat/src/failed.c
kis_file.o : kissat/src/file.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_file.o kissat/src/file.c
kis_flags.o : kissat/src/flags.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_flags.o kissat/src/flags.c
kis_format.o : kissat/src/format.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_format.o kissat/src/format.c
kis_forward.o : kissat/src/forward.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_forward.o kissat/src/forward.c
kis_frames.o : kissat/src/frames.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_frames.o kissat/src/frames.c
kis_gates.o : kissat/src/gates.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_gates.o kissat/src/gates.c
kis_handle.o : kissat/src/handle.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_handle.o kissat/src/handle.c
kis_heap.o : kissat/src/heap.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_heap.o kissat/src/heap.c
kis_ifthenelse.o : kissat/src/ifthenelse.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_ifthenelse.o kissat/src/ifthenelse.c
kis_import.o : kissat/src/import.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_import.o kissat/src/import.c
kis_internal.o : kissat/src/internal.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_internal.o kissat/src/internal.c
kis_learn.o : kissat/src/learn.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_learn.o kissat/src/learn.c
kis_limits.o : kissat/src/limits.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_limits.o kissat/src/limits.c
kis_logging.o : kissat/src/logging.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_logging.o kissat/src/logging.c
kis_main.o : kissat/src/main.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_main.o kissat/src/main.c
kis_minimize.o : kissat/src/minimize.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_minimize.o kissat/src/minimize.c
kis_mode.o : kissat/src/mode.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_mode.o kissat/src/mode.c
kis_options.o : kissat/src/options.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_options.o kissat/src/options.c
kis_parse.o : kissat/src/parse.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_parse.o kissat/src/parse.c
kis_phases.o : kissat/src/phases.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_phases.o kissat/src/phases.c
kis_print.o : kissat/src/print.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_print.o kissat/src/print.c
kis_probe.o : kissat/src/probe.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_probe.o kissat/src/probe.c
kis_profile.o : kissat/src/profile.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_profile.o kissat/src/profile.c
kis_promote.o : kissat/src/promote.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_promote.o kissat/src/promote.c
kis_proof.o : kissat/src/proof.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_proof.o kissat/src/proof.c
kis_propdense.o : kissat/src/propdense.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_propdense.o kissat/src/propdense.c
kis_prophyper.o : kissat/src/prophyper.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_prophyper.o kissat/src/prophyper.c
kis_proprobe.o : kissat/src/proprobe.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_proprobe.o kissat/src/proprobe.c
kis_propsearch.o : kissat/src/propsearch.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_propsearch.o kissat/src/propsearch.c
kis_queue.o : kissat/src/queue.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_queue.o kissat/src/queue.c
kis_reduce.o : kissat/src/reduce.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_reduce.o kissat/src/reduce.c
kis_reluctant.o : kissat/src/reluctant.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_reluctant.o kissat/src/reluctant.c
kis_rephase.o : kissat/src/rephase.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_rephase.o kissat/src/rephase.c
kis_report.o : kissat/src/report.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_report.o kissat/src/report.c
kis_resize.o : kissat/src/resize.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_resize.o kissat/src/resize.c
kis_resolve.o : kissat/src/resolve.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_resolve.o kissat/src/resolve.c
kis_resources.o : kissat/src/resources.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_resources.o kissat/src/resources.c
kis_restart.o : kissat/src/restart.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_restart.o kissat/src/restart.c
kis_search.o : kissat/src/search.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_search.o kissat/src/search.c
kis_smooth.o : kissat/src/smooth.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_smooth.o kissat/src/smooth.c
kis_sort.o : kissat/src/sort.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_sort.o kissat/src/sort.c
kis_stack.o : kissat/src/stack.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_stack.o kissat/src/stack.c
kis_statistics.o : kissat/src/statistics.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_statistics.o kissat/src/statistics.c
kis_strengthen.o : kissat/src/strengthen.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_strengthen.o kissat/src/strengthen.c
kis_substitute.o : kissat/src/substitute.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_substitute.o kissat/src/substitute.c
kis_terminate.o : kissat/src/terminate.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_terminate.o kissat/src/terminate.c
kis_ternary.o : kissat/src/ternary.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_ternary.o kissat/src/ternary.c
kis_trail.o : kissat/src/trail.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_trail.o kissat/src/trail.c
kis_transitive.o : kissat/src/transitive.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_transitive.o kissat/src/transitive.c
kis_utilities.o : kissat/src/utilities.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_utilities.o kissat/src/utilities.c
kis_vector.o : kissat/src/vector.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_vector.o kissat/src/vector.c
kis_vivify.o : kissat/src/vivify.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_vivify.o kissat/src/vivify.c
kis_walk.o : kissat/src/walk.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_walk.o kissat/src/walk.c
kis_watch.o : kissat/src/watch.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_watch.o kissat/src/watch.c
kis_weaken.o : kissat/src/weaken.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_weaken.o kissat/src/weaken.c
kis_witness.o : kissat/src/witness.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_witness.o kissat/src/witness.c
kis_xors.o : kissat/src/xors.c
	$(CCC) $(KISSAT_FLAGS)  -o kis_xors.o kissat/src/xors.c
picat_utilities.o : picat_utilities.c
	$(CCC) $(CFLAGS) picat_utilities.c
fann.o : fann/src/fann.c
	$(CCC) $(CFLAGS) -Ifann/src/include fann/src/fann.c
fann_cascade.o : fann/src/fann_cascade.c
	$(CCC) $(CFLAGS) -Ifann/src/include fann/src/fann_cascade.c
fann_error.o : fann/src/fann_error.c
	$(CCC) $(CFLAGS) -Ifann/src/include fann/src/fann_error.c
fann_io.o : fann/src/fann_io.c
	$(CCC) $(CFLAGS) -Ifann/src/include fann/src/fann_io.c
fann_train.o : fann/src/fann_train.c
	$(CCC) $(CFLAGS) -Ifann/src/include fann/src/fann_train.c
fann_train_data.o : fann/src/fann_train_data.c
	$(CCC) $(CFLAGS) -Ifann/src/include fann/src/fann_train_data.c
fann_interface.o : fann/fann_interface.cpp
	$(CPPC) $(CFLAGS) -Ifann/src/include fann/fann_interface.cpp
bp_pcre2.o: bp_pcre2.c
	$(CCC) $(CFLAGS) bp_pcre2.c
