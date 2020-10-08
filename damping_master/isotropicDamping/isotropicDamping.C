/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2019 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "isotropicDamping.H"
#include "fvMatrix.H"
#include "fvmSup.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{
    defineTypeNameAndDebug(isotropicDamping, 0);
    addToRunTimeSelectionTable(option, isotropicDamping, dictionary);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fv::isotropicDamping::add
(
    const volScalarField::Internal& forceCoeff,
    fvMatrix<vector>& eqn
)
{
    //Info <<"forceCoeff"<<max(forceCoeff)<<"\n"<<endl;
    //Info <<"Dimensions_forceCoeff"<<forceCoeff.dimensions()<<"\n"<<endl;
    //Info <<"Dimensions_value"<<value_.dimensions()<<"\n"<<endl;
    //Info <<"Dimensions_eqn"<<eqn.dimensions()<<"\n"<<endl;
    //Info <<"Dimensions_eqn.psi"<<eqn.psi().dimensions()<<"\n"<<endl;
    eqn -= fvm::Sp(forceCoeff, eqn.psi());
    eqn += forceCoeff*value_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fv::isotropicDamping::isotropicDamping
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    damping(name, modelType, dict, mesh),
    value_("value", dimVelocity, coeffs_.lookup("value"))
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fv::isotropicDamping::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    //Info<<"i am here 1"<<endl;
    add(this->forceCoeff(), eqn);
}


void Foam::fv::isotropicDamping::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    //Info<<"i am here 2"<<endl;
    //Info<<"rho"<<min(rho);
    add(rho*forceCoeff(), eqn);
}


void Foam::fv::isotropicDamping::addSup
(
    const volScalarField& alpha,
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    //Info<<"i am here 3"<<endl;
    add(alpha()*rho()*this->forceCoeff(), eqn);
}


bool Foam::fv::isotropicDamping::read(const dictionary& dict)
{
    if (damping::read(dict))
    {
        value_ =
            dimensionedVector
            (
                value_.name(),
                value_.dimensions(),
                coeffs_.lookup(value_.name())
            );

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
