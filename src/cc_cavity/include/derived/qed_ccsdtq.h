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

#ifndef CC_CAVITY_QED_CCSDTQ_H
#define CC_CAVITY_QED_CCSDTQ_H

#include "qed_ccsdt.h"

using namespace std;
using namespace TA;
using namespace psi;
using namespace Helper;

namespace hilbert {

    class QED_CCSDTQ : public QED_CCSDT {

    public:
        QED_CCSDTQ(const shared_ptr<Wavefunction> &reference_wavefunction, Options & options);
        ~QED_CCSDTQ() override = default;

    protected:

        /**
         * @brief Initialize the CC operators
         */
        virtual void init_operators() override;

        /**
         * @brief Print the header for the iteration
         */
        virtual void print_iter_header() const override;

        /**
         * @brief Print the iteration information
         * @param iter number of current iteration
         * @param energy current energy
         * @param dele energy change
         * @param tnorm current residual norm
         */
        virtual void print_iteration(size_t iter, double energy, double dele, double tnorm) const override;

        /**
         * @brief Build the residual vectors and compute the energy
         * @return the energy
         */
        virtual double build_residuals() override;

        /**
         * @brief Update the amplitudes
         */
        virtual void update_amplitudes() override;

        /**
         * @brief compute the residual norms for each operator
         * @return the total residual norm
         */
        virtual double compute_residual_norms(bool return_tot = true) override;

        /**
         * @brief Print summary of amplitude properties
         */
        virtual void print_properties() override;

    };

} // cc_cavity

#endif //CC_CAVITY_QED_CCSDTQ_H