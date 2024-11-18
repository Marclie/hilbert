/*
 *  @BEGIN LICENSE
 *
 *  Hilbert: a space for quantum chemistry plugins to Psi4
 *
 *  Copyright (c) 2020 by its authors (LICENSE).
 *
 *  The copyrights for code used from other parties are included in
 *  the corresponding files.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  @END LICENSE
 */

#include "cc_cavity/include/ccsd/eom_ea_ccsd.h"

void hilbert::EOM_EA_CCSD::sigma_ea_00_2() {

    // get reference to electronic integrals
    TArrayMap &V_blks_ = cc_wfn_->V_blks_;
    TArrayMap &F_blks_ = cc_wfn_->F_blks_;
    TArrayMap &Id_blks_ = cc_wfn_->Id_blks_;
    TArrayMap &eri = cc_wfn_->V_blks_;
    TArrayMap &f = cc_wfn_->F_blks_;
    TArrayMap &Id = cc_wfn_->Id_blks_;

    TArrayMap t2 = {
            {"aaaa", cc_wfn_->amplitudes_["t2_aaaa"]},
            {"abab", cc_wfn_->amplitudes_["t2_abab"]},
            {"bbbb", cc_wfn_->amplitudes_["t2_bbbb"]}
    };

    /// reference right operators
    TArrayMap r1 = {
            {"a", evec_blks_["r1_a"]},
            {"b", evec_blks_["r1_b"]}
    };

    TArrayMap r2 = {
            {"aaa", evec_blks_["r2_aaa"]},
            {"abb", evec_blks_["r2_abb"]},
            {"aba", evec_blks_["r2_aba"]},
            {"bbb", evec_blks_["r2_bbb"]}
    };

    /// reference left operators

    TArrayMap l1 = {
            {"a", evec_blks_["l1_a"]},
            {"b", evec_blks_["l1_b"]}
    };
    TArrayMap l2 = {
            {"aaa", evec_blks_["l2_aaa"]},
            {"bab", evec_blks_["l2_bab"]},
            {"aab", evec_blks_["l2_aab"]},
            {"bbb", evec_blks_["l2_bbb"]}
    };

    /// reference sigma vectors
    auto &sigmar1_a = sigvec_blks_["sigmar1_a"];
    auto &sigmar1_b = sigvec_blks_["sigmar1_b"];
    auto &sigmal1_a = sigvec_blks_["sigmal1_a"];
    auto &sigmal1_b = sigvec_blks_["sigmal1_b"];

    auto &sigmar2_aaa = sigvec_blks_["sigmar2_aaa"];
    auto &sigmar2_abb = sigvec_blks_["sigmar2_abb"];
    auto &sigmal2_aaa = sigvec_blks_["sigmal2_aaa"];
    auto &sigmal2_abb = sigvec_blks_["sigmal2_abb"];


    // flops: o1v2L1  = o1v2L1
    //  mems: o1v2L1  = o1v2L1
    tmps_["11_abb_Lvvo"]("R,a,b,i")  = -1.00 * r1["a"]("R,a") * reused_["14_bb_vo"]("b,i");

    // flops: o1v2L1  = o2v3L1
    //  mems: o1v2L1  = o1v2L1
    tmps_["10_bab_Lvov"]("R,a,i,b")  = eri["abab_oovv"]("i,j,c,b") * r2["abb"]("R,c,a,j");

    // flops: o0v1L1  = o1v3 o1v3 o1v3L1 o1v3L1 o0v1L1 o1v3L1 o0v1L1 o0v2L1 o0v1L1 o1v2L1 o0v1L1 o1v2L1 o0v1L1 o1v2L1 o0v1L1 o1v2L1 o0v1L1 o1v3L1 o0v1L1 o0v1L1 o0v1L1 o1v3L1 o0v1L1 o1v3L1 o0v1L1 o0v2L1 o0v1L1 o0v2L1 o0v1L1 o0v1L1
    //  mems: o0v1L1  = o1v3 o1v3 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1
    tmps_["8_a_Lv"]("L,a")  = -0.50 * (4.00 * reused_["5_aaaa_vovv"]("b,k,e,a") + reused_["10_aaaa_vovv"]("a,k,e,b") + 2.00 * eri["aaaa_vvvo"]("b,e,a,k")) * l2["aaa"]("L,k,e,b");
    tmps_["8_a_Lv"]("L,a") += 2.00 * eri["abab_vvvo"]("e,f,a,i") * l2["bab"]("L,i,e,f");
    tmps_["8_a_Lv"]("L,a") += 2.00 * l2["bab"]("L,i,e,f") * reused_["7_aabb_vvvo"]("e,a,f,i");
    tmps_["8_a_Lv"]("L,a") -= 2.00 * l1["a"]("L,b") * reused_["4_aa_vv"]("b,a");
    tmps_["8_a_Lv"]("L,a") += 2.00 * l2["bab"]("L,i,a,f") * reused_["14_bb_vo"]("f,i");
    tmps_["8_a_Lv"]("L,a") -= 2.00 * f["aa_vo"]("b,k") * l2["aaa"]("L,k,b,a");
    tmps_["8_a_Lv"]("L,a") += 2.00 * f["bb_vo"]("f,i") * l2["bab"]("L,i,a,f");
    tmps_["8_a_Lv"]("L,a") -= l2["aaa"]("L,k,b,a") * reused_["13_aa_ov"]("k,b");
    tmps_["8_a_Lv"]("L,a") -= 2.00 * l2["bab"]("L,i,e,f") * reused_["8_aabb_vvvo"]("e,a,f,i");
    tmps_["8_a_Lv"]("L,a") -= scalars_["1"] * l1["a"]("L,a");
    tmps_["8_a_Lv"]("L,a") += 2.00 * l2["bab"]("L,i,e,f") * reused_["11_abab_vovv"]("a,i,e,f");
    tmps_["8_a_Lv"]("L,a") += 2.00 * l2["aaa"]("L,k,e,b") * reused_["6_aaaa_vvvo"]("e,a,b,k");
    tmps_["8_a_Lv"]("L,a") += 2.00 * f["aa_vv"]("b,a") * l1["a"]("L,b");
    tmps_["8_a_Lv"]("L,a") -= l1["a"]("L,b") * reused_["12_aa_vv"]("a,b");
    tmps_["8_a_Lv"]("L,a") -= 0.50 * tmps_["6_a_Lv"]("L,a");
    tmps_["8_a_Lv"].~TArrayD();
    tmps_["6_a_Lv"].~TArrayD();

    // flops: o3v0L1  = o3v2L1
    //  mems: o3v0L1  = o3v0L1
    tmps_["9_abb_Looo"]("R,i,j,k")  = eri["abab_oovv"]("i,j,a,b") * r2["abb"]("R,a,b,k");

    // flops: o1v2L1  = o2v2L1 o3v2L1 o1v3L1 o2v3L1 o1v2L1 o1v3L1 o1v2L1 o1v3L1 o1v2L1 o1v3L1 o1v2L1 o2v2L1 o1v2L1 o1v4L1 o1v2L1 o1v3L1 o1v2L1 o2v2L1 o1v2L1 o2v3L1 o1v2L1 o2v3L1 o1v2L1 o1v3L1 o1v2L1 o1v2L1 o1v2L1 o1v3L1 o1v2L1 o2v3L1 o1v2L1 o1v3L1 o1v2L1 o2v3L1 o1v2L1 o2v3L1 o1v2L1 o1v3L1 o1v2L1 o1v3L1 o1v2L1 o2v2L1 o1v2L1 o1v2L1 o1v2L1 o1v3L1 o1v2L1 o2v3L1 o1v2L1 o1v2L1 o2v3L1 o1v2L1 o1v2L1 o1v2L1
    //  mems: o1v2L1  = o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1 o1v2L1
    tmps_["13_abb_Lvov"]("R,a,i,b")  = -1.00 * t2["abab"]("a,b,l,i") * tmps_["12_a_Lo"]("R,l");
    tmps_["13_abb_Lvov"]("R,a,i,b") += t2["abab"]("a,b,j,k") * tmps_["9_abb_Looo"]("R,j,k,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["abb"]("R,f,b,i") * reused_["4_aa_vv"]("a,f");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r2["aaa"]("R,d,a,l") * eri["baab_vovo"]("b,l,d,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += f["aa_vv"]("a,d") * r2["abb"]("R,d,b,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["abb"]("R,a,e,i") * reused_["1_bb_vv"]("e,b");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r1["a"]("R,f") * reused_["9_baab_vvvo"]("b,f,a,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["abb"]("R,a,b,k") * f["bb_oo"]("k,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += eri["abab_vvvv"]("a,b,d,e") * r2["abb"]("R,d,e,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r2["abb"]("R,a,c,i") * f["bb_vv"]("b,c");
    tmps_["13_abb_Lvov"]("R,a,i,b") += 0.50 * r2["abb"]("R,a,b,m") * reused_["18_bb_oo"]("i,m");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["abb"]("R,a,c,k") * eri["bbbb_vovo"]("b,k,c,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["aaa"]("R,f,a,j") * reused_["16_bbaa_voov"]("b,i,j,f");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r1["a"]("R,f") * reused_["15_aabb_vvvo"]("a,f,b,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r1["a"]("R,a") * f["bb_vo"]("b,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r1["a"]("R,f") * reused_["8_aabb_vvvo"]("a,f,b,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r2["abb"]("R,a,e,m") * reused_["19_bbbb_voov"]("b,i,m,e");
    tmps_["13_abb_Lvov"]("R,a,i,b") += 0.50 * r2["abb"]("R,f,b,i") * reused_["3_aa_vv"]("a,f");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["abb"]("R,a,e,m") * reused_["17_bbbb_voov"]("b,i,m,e");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= eri["abab_vovo"]("a,k,d,i") * r2["abb"]("R,d,b,k");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r1["a"]("R,d") * reused_["11_abab_vovv"]("d,i,a,b");
    tmps_["13_abb_Lvov"]("R,a,i,b") += 0.50 * r2["abb"]("R,a,e,i") * reused_["2_bb_vv"]("b,e");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= r2["abb"]("R,a,b,m") * reused_["21_bb_oo"]("i,m");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= 0.50 * scalars_["1"] * r2["abb"]("R,a,b,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += eri["abab_vvvo"]("a,b,d,i") * r1["a"]("R,d");
    tmps_["13_abb_Lvov"]("R,a,i,b") += r2["aaa"]("R,f,a,j") * reused_["20_bbaa_voov"]("b,i,j,f");
    tmps_["13_abb_Lvov"]("R,a,i,b") -= tmps_["11_abb_Lvvo"]("R,a,b,i");
    tmps_["13_abb_Lvov"]("R,a,i,b") += t2["abab"]("a,c,l,i") * tmps_["10_bab_Lvov"]("R,b,l,c");
    tmps_["11_abb_Lvvo"].~TArrayD();
    tmps_["10_bab_Lvov"].~TArrayD();
    tmps_["9_abb_Looo"].~TArrayD();

    // sigmar2_abb  = +1.00 <j,k||d,c>_abab r2_abb(d,b,k) t2_abab(a,c,j,i)
    //             += -0.50 <k,j||d,c>_abab r2_abb(d,b,i) t2_abab(a,c,k,j)
    //             += -0.50 <j,k||d,c>_abab r2_abb(d,b,i) t2_abab(a,c,j,k)
    //             += -1.00 <j,b||c,i>_abab r2_aaa(c,a,j)
    //             += +1.00 f_aa(a,c) r2_abb(c,b,i)
    //             += -0.50 <k,j||c,d>_abab r2_abb(a,d,i) t2_abab(c,b,k,j)
    //             += -0.50 <j,k||c,d>_abab r2_abb(a,d,i) t2_abab(c,b,j,k)
    //             += -1.00 <j,b||d,c>_abab r1_a(d) t2_abab(a,c,j,i)
    //             += -1.00 f_bb(j,i) r2_abb(a,b,j)
    //             += +0.50 <a,b||c,d>_abab r2_abb(c,d,i)
    //             += +0.50 <a,b||d,c>_abab r2_abb(d,c,i)
    //             += +1.00 f_bb(b,c) r2_abb(a,c,i)
    //             += -0.50 <k,j||c,d>_bbbb r2_abb(a,b,k) t2_bbbb(c,d,i,j)
    //             += +1.00 <j,b||c,i>_bbbb r2_abb(a,c,j)
    //             += +1.00 <k,j||c,d>_aaaa r2_aaa(d,a,k) t2_abab(c,b,j,i)
    //             += +1.00 <j,a||c,d>_aaaa r1_a(d) t2_abab(c,b,j,i)
    //             += +1.00 f_bb(b,i) r1_a(a)
    //             += -1.00 <a,j||d,c>_abab r1_a(d) t2_bbbb(c,b,i,j)
    //             += +1.00 <j,k||c,d>_abab r2_abb(a,d,k) t2_abab(c,b,j,i)
    //             += -0.50 <k,j||c,d>_aaaa r2_abb(d,b,i) t2_aaaa(c,a,k,j)
    //             += +1.00 <k,j||c,d>_bbbb r2_abb(a,d,k) t2_bbbb(c,b,i,j)
    //             += -1.00 <a,j||c,i>_abab r2_abb(c,b,j)
    //             += +0.50 <k,j||c,i>_abab r1_a(c) t2_abab(a,b,k,j)
    //             += +0.50 <j,k||c,i>_abab r1_a(c) t2_abab(a,b,j,k)
    //             += -0.50 <k,j||c,d>_bbbb r2_abb(a,d,i) t2_bbbb(c,b,k,j)
    //             += -0.50 <j,k||c,d>_abab r2_abb(a,b,k) t2_abab(c,d,j,i)
    //             += -0.50 <j,k||d,c>_abab r2_abb(a,b,k) t2_abab(d,c,j,i)
    //             += -0.50 <k,j||k,j>_bbbb r2_abb(a,b,i)
    //             += -0.50 <k,j||k,j>_aaaa r2_abb(a,b,i)
    //             += -0.50 <k,j||k,j>_abab r2_abb(a,b,i)
    //             += -0.50 <j,k||j,k>_abab r2_abb(a,b,i)
    //             += +0.25 <k,j||c,d>_bbbb r2_abb(a,b,i) t2_bbbb(c,d,k,j)
    //             += +0.25 <k,j||c,d>_abab r2_abb(a,b,i) t2_abab(c,d,k,j)
    //             += +0.25 <j,k||c,d>_abab r2_abb(a,b,i) t2_abab(c,d,j,k)
    //             += +0.25 <k,j||d,c>_abab r2_abb(a,b,i) t2_abab(d,c,k,j)
    //             += +0.25 <j,k||d,c>_abab r2_abb(a,b,i) t2_abab(d,c,j,k)
    //             += +1.00 f_bb(j,j) r2_abb(a,b,i)
    //             += +0.25 <k,j||c,d>_aaaa r2_abb(a,b,i) t2_aaaa(c,d,k,j)
    //             += +1.00 f_aa(j,j) r2_abb(a,b,i)
    //             += +1.00 <a,b||c,i>_abab r1_a(c)
    //             += +1.00 <k,j||d,c>_abab r2_aaa(d,a,k) t2_bbbb(c,b,i,j)
    //             += +1.00 f_aa(j,c) r1_a(a) t2_abab(c,b,j,i)
    //             += -0.50 <j,b||c,d>_bbbb r1_a(a) t2_bbbb(c,d,i,j)
    //             += +0.50 <j,b||c,d>_abab r1_a(a) t2_abab(c,d,j,i)
    //             += +0.50 <j,b||d,c>_abab r1_a(a) t2_abab(d,c,j,i)
    //             += -1.00 f_bb(j,c) r1_a(a) t2_bbbb(c,b,i,j)
    //             += -0.50 <k,j||c,i>_bbbb r1_a(a) t2_bbbb(c,b,k,j)
    //             += -0.50 <k,j||c,i>_abab r1_a(a) t2_abab(c,b,k,j)
    //             += -0.50 <j,k||c,i>_abab r1_a(a) t2_abab(c,b,j,k)
    //             += +0.25 <k,j||c,d>_abab r2_abb(c,d,i) t2_abab(a,b,k,j)
    //             += +0.25 <j,k||c,d>_abab r2_abb(c,d,i) t2_abab(a,b,j,k)
    //             += +0.25 <k,j||d,c>_abab r2_abb(d,c,i) t2_abab(a,b,k,j)
    //             += +0.25 <j,k||d,c>_abab r2_abb(d,c,i) t2_abab(a,b,j,k)
    //             += -1.00 f_aa(j,c) r1_a(c) t2_abab(a,b,j,i)
    //             += -0.50 <j,k||c,d>_abab r2_abb(c,d,k) t2_abab(a,b,j,i)
    //             += -0.50 <j,k||d,c>_abab r2_abb(d,c,k) t2_abab(a,b,j,i)
    //             += +0.50 <k,j||c,d>_aaaa r2_aaa(c,d,k) t2_abab(a,b,j,i)
    sigmar2_abb("R,a,b,i")  = tmps_["13_abb_Lvov"]("R,a,i,b");
    tmps_["13_abb_Lvov"].~TArrayD();

    // flops: o1v2L1  = o2v2L1
    //  mems: o1v2L1  = o1v2L1
    tmps_["14_abb_Lvvo"]("R,a,b,i")  = -1.00 * t2["abab"]("a,b,j,i") * tmps_["12_a_Lo"]("R,j");
    tmps_["14_abb_Lvvo"].~TArrayD();

    // flops: o1v0L1  = o2v2L1 o2v2L1 o1v0L1
    //  mems: o1v0L1  = o1v0L1 o1v0L1 o1v0L1
    tmps_["15_b_Lo"]("L,i")  = 0.50 * l2["bbb"]("L,k,c,b") * t2["bbbb"]("c,b,k,i");
    tmps_["15_b_Lo"]("L,i") += l2["aab"]("L,j,a,b") * t2["abab"]("a,b,j,i");

    // flops: o0v1L1  = o1v2L1 o0v2L1 o0v1L1 o1v2L1 o0v1L1 o0v2L1 o0v1L1 o0v1L1 o0v1L1 o1v2L1 o0v1L1 o1v3L1 o0v1L1 o1v3L1 o0v1L1 o1v3L1 o0v1L1 o0v2L1 o0v1L1 o1v3L1 o0v1L1 o1v3L1 o0v1L1 o1v2L1 o0v1L1 o1v1L1 o0v1L1
    //  mems: o0v1L1  = o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1 o0v1L1
    tmps_["16_b_Lv"]("L,a")  = -1.00 * f["aa_vo"]("f,j") * l2["aab"]("L,j,f,a");
    tmps_["16_b_Lv"]("L,a") -= 0.50 * l1["b"]("L,c") * reused_["22_bb_vv"]("a,c");
    tmps_["16_b_Lv"]("L,a") -= 0.50 * l2["aab"]("L,j,f,a") * reused_["13_aa_ov"]("j,f");
    tmps_["16_b_Lv"]("L,a") += f["bb_vv"]("c,a") * l1["b"]("L,c");
    tmps_["16_b_Lv"]("L,a") -= 0.50 * scalars_["1"] * l1["b"]("L,a");
    tmps_["16_b_Lv"]("L,a") -= f["bb_vo"]("c,k") * l2["bbb"]("L,k,c,a");
    tmps_["16_b_Lv"]("L,a") += eri["abba_vvvo"]("b,c,a,j") * l2["aab"]("L,j,b,c");
    tmps_["16_b_Lv"]("L,a") -= l2["aab"]("L,j,f,d") * reused_["24_aabb_vovv"]("f,j,d,a");
    tmps_["16_b_Lv"]("L,a") -= l2["bbb"]("L,k,d,c") * reused_["25_bbbb_vovv"]("c,k,d,a");
    tmps_["16_b_Lv"]("L,a") -= l1["b"]("L,c") * reused_["1_bb_vv"]("a,c");
    tmps_["16_b_Lv"]("L,a") -= 0.50 * eri["bbbb_vvvo"]("c,d,a,k") * l2["bbb"]("L,k,d,c");
    tmps_["16_b_Lv"]("L,a") += l2["aab"]("L,j,b,c") * reused_["23_abba_vvvo"]("b,a,c,j");
    tmps_["16_b_Lv"]("L,a") -= l2["bbb"]("L,k,c,a") * reused_["14_bb_vo"]("c,k");
    tmps_["16_b_Lv"]("L,a") += f["bb_ov"]("i,a") * tmps_["15_b_Lo"]("L,i");
    tmps_["15_b_Lo"].~TArrayD();
}