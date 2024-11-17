#pragma once

#include <MathSupport.h>
#include <vector3.h>
#include <Matrix3x3.h>

class MassChannel
{
public:

	enum Stage
	{
		Collecting,
		Distributing
	};

	MassChannel() :_mass(0.0f)
	{
		Reset();
	}

	MassChannel( float mass, Vector3F &cgOffset ) :
		_mass(mass), _cgOffset( cgOffset )
	{
		Reset();
	}

	void setMass(float fMass)
	{
		_mass = fMass;
	}

	void setCGOffset( Vector3F cgOffset )
	{
		_cgOffset = cgOffset;
	}

	bool isDistributing()
	{
		return _stage == Distributing;
	}

	void Reset()
	{
		_MinLeft.Reset();
		_MinRight.Reset();
		_massDistance.Reset();
		_MOI.Reset();
		_ileft_count = 0.0f;
		_iright_count = 0.0f;
		_sigleft = 0.0f;
		_sigright = 0.0f;
		_c1 = 0.0f;
		_c2 = 0.0f;
		_sig = 0.0f;
		_i_i = 0.0f;
		_stage = Collecting;
	}

	void AddPoint( float x, float y, float z )
	{
		AddPoint( Vector3F(x,y,z) );
	}

	void AddPoint( Vector3F v, Vector3F vVol = Vector3F() )
	{
		if( _stage == Collecting )
		{
			v -= _cgOffset;	
			if( !v ) return;
			isleft(v) ? AddLeftPoint(v) : AddRightPoint(v);
		}
		else
			CalcMOI( v, vVol );
	}

	float GetMass(Vector3F v) const
	{
		v -= _cgOffset;
		if( !v ) return 0.0f;
		return (isleft(v) ? 1.0f : _i_i  ) * _mass / (v.Magnitude() * _sig );
	}

	float GetMass(float x, float y, float z) const
	{
		return GetMass( Vector3F( x,y,z ) );
	}

	void Distribute()
	{
		if( _iright_count == 0.0f )
		{
			_sig = _sigleft;
			_c1 = 1.0f / (_MinLeft.Magnitude() * _sig );
			_i_i = 1.0f;
		}
		else if( _ileft_count == 0 )
		{
			_sig = _sigright;
			_c2 = 1.0f / (_MinRight.Magnitude() * _sig );
			_i_i = 1.0f;
		}
		else
		{
			_i_i = _ileft_count / _iright_count;
			_sig = _sigleft + _i_i *_sigright;
			_c1 = 1.0f / (_MinLeft.Magnitude() * _sig );
			_c2 = _i_i / (_MinRight.Magnitude() * _sig );
		}

		_MOI.Reset();
		_stage = Distributing;
	}

	Vector3F CgOffset() const
	{
		return _cgOffset;
	}

        void setMOIFactor(float f)
        {
            _moiFactor = f;
        }

	Vector3F CG() const
	{
		return _massDistance/TotalMass() + CgOffset();
	}

	Matrix3x3F MOI() const
	{
                return _MOI * _moiFactor;
	}

	float TotalMass() const 
	{ 
		return _mass; 
	}

	void CalcCG(Vector3F pt)
	{
		_massDistance += GetMass(pt) * ( pt-_cgOffset );
	}

	void CalcInvMOI()
	{
                _INVMOI = MOI().Inverse();
	}

protected:
	void CalcMOI(Vector3F pt, const Vector3F &vVol)
	{
		float fMass = GetMass( pt );
		pt -= CG();
		
		float xx = vVol.x * vVol.x;
		float yy = vVol.y * vVol.y;
		float zz = vVol.z * vVol.z;

		// Local INertia for Small Volume Block
		float fIxx = fMass * (yy + zz) /12.0f;
		float fIyy = fMass * (xx + zz) /12.0f;
		float fIzz = fMass * (xx + yy) /12.0f;

		float mr2xx = pt.x * pt.x;
		float mr2yy = pt.y * pt.y;
		float mr2zz = pt.z * pt.z;

		// Using parallel axis theorem to calculate I at C/G : I= I0+mr^2
		_MOI.e11 += fIxx + fMass * ( mr2yy + mr2zz );
		_MOI.e22 += fIyy + fMass * ( mr2xx + mr2zz );
		_MOI.e33 += fIzz + fMass * ( mr2xx + mr2yy );

		// Mixing
		float fIXY = fMass * ( pt.x * pt.y );
		float fIYZ = fMass * ( pt.y * pt.z );
		float fIZX = fMass * ( pt.z * pt.x );

		_MOI.e12 -= fIXY;
		_MOI.e21 -= fIXY;

		_MOI.e13 -= fIZX;
		_MOI.e31 -= fIZX;

		_MOI.e23 -= fIYZ;
		_MOI.e32 -= fIYZ;
	}

	bool isleft( const Vector3F &pt ) const
	{
		return pt.z > 0.0f ? true : ( pt.z < 0.0f ? false : ( pt.x > 0.0f ? true : ( pt.x < 0.0f ? false : ( pt.y > 0.0f ? true : false ) ) ) );
	}

	void AddLeftPoint( Vector3F &pt ) 	
	{
		_ileft_count++;
		AddPoint( &MassChannel::_MinLeft, &MassChannel::_sigleft, pt );
	}

	void AddRightPoint( Vector3F &pt ) 
	{
		_iright_count++;
		AddPoint( &MassChannel::_MinRight, &MassChannel::_sigright, pt );
	}

	void AddPoint( Vector3F (MassChannel::*MinContact), float (MassChannel::*sigptr), Vector3F &pt )
	{
		if( this->*MinContact == Vector3F() ) 
			this->*MinContact = pt;
		else if( pt < this->*MinContact )
			this->*MinContact = pt;

		this->*sigptr += 1.0f / pt.Magnitude();
	}

private:
	Stage _stage;
	Vector3F _cgOffset;
	Vector3F _MinLeft;
	Vector3F _MinRight;
	Vector3F _massDistance;
	Matrix3x3F _MOI;
	Matrix3x3F _INVMOI;
	float _mass;
	float _ileft_count;
	float _iright_count;
	float _sigleft;
	float _sigright;
	float _c1;
	float _c2;
	float _sig;
	float _i_i;
        float _moiFactor = 1.0f;
};

