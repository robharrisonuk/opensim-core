#ifndef OPENSIM_FRAME_H_
#define OPENSIM_FRAME_H_
/* -------------------------------------------------------------------------- *
 *                              OpenSim:  Frame.h                             *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2014 Stanford University and the Authors                *
 * Author(s): Matt DeMers, Ajay Seth, Ayman Habib                             *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

// INCLUDE
#include <OpenSim/Simulation/osimSimulationDLL.h>
#include <OpenSim/Simulation/Model/ModelComponent.h>

namespace OpenSim {

//=============================================================================
//=============================================================================
/**
 * A Frame is an OpenSim abstraction for a right-handed set of orthogonal axes.
 * It is a reference frame. Frames are intended to provide convenient reference
 * frames for locating phyical structures (such as joints and muscle
 * attachments) as well as provide a convenient basis for performing spatial 
 * calculations such as a frame aligned with the normal direction of a contact
 * surface or located at the center-of-pressure.
 *
 * @author Matt DeMers
 * @author Ajay Seth
 */
class OSIMSIMULATION_API Frame : public ModelComponent {
OpenSim_DECLARE_ABSTRACT_OBJECT(Frame, ModelComponent);

public:
    //--------------------------------------------------------------------------
    // CONSTRUCTION
    //--------------------------------------------------------------------------
    Frame();

    virtual ~Frame() {};
    /** @name Spatial Operations for Frames
    These methods allow access to the frame's transform and some convenient
    operations that could be performed with this transform.*/
    /**@{**/

    /**
    Get the transform of this frame (F) relative to the ground frame (G).
    The transform transforms quantities expressed in F to quantities expressed
    in G. This is mathematically stated as:
        vec_G = X_GF*vec_F ,
    where X_GF is the transform returned by getGroundTransform.

    @param state       The state applied to the model when determining the
                       transform.
    @return transform  The transform between this frame and the ground frame
    */
    const SimTK::Transform& getGroundTransform(const SimTK::State& state) const {
        return calcGroundTransform(state);
    }

    /**
    Get the transform that describes the translation and rotation of this
    frame (F frame) relative to another frame (A frame).  This method returns
    the transform converting quantities expressed in F frame to quantities
    expressed in the A frame. This is mathematically stated as:
        vec_A = X_AF*vec_F ,
    where X_AF is the transform returned by this method.

    @param state       The state applied to the model when determining the
                       transform.
    @param otherFrame  a second frame
    @return transform  The transform between this frame and otherFrame
    */
    SimTK::Transform findTransformBetween(const SimTK::State& state,
                                          const Frame& otherFrame) const;

    /**
    Take a vector expressed in this frame (F) re-express the same vector 
    in another frame (A).  This re-expression accounts for the difference
    in orientation between the frames. This is mathematically stated as:
        vec_A = R_AF*vec  
    and thus is does not translate the vector. This is intended to reexpress
    physical qunatities such as angular velocity
    THIS METHOD DOES NOT PERFORM A HOMOGENOUS TRANSFORM

    @param state       The state of the model.
    @param vec         The vector to be re-expressed.
    @param otherFrame  The frame in which the vector will be re-expressed
    @return newVec     The expression of the vector in otherFrame.
    */
    SimTK::Vec3 expressVectorInAnotherFrame(const SimTK::State& state,
                        const SimTK::Vec3& vec, const Frame& otherFrame) const;

    /**
    Take a point located and expressed in this frame (F) and determine
    its location expressed in another frame (A) using the homogeneous
    transformation. That is the transformation accounts for the difference in
    orientation and translation between the frames. In mathematical form,
    this method returns point_A, where point_A = X_AF*point_F.

    @param state       The state of the model.
    @param point       The point to be re-expressed.
    @param otherFrame  The frame in which the point will be re-expressed
    @return point_A    The re-expression of the point in another frame.
    */
    SimTK::Vec3 findLocationInAnotherFrame(const SimTK::State& state, 
                    const SimTK::Vec3& point, const Frame& otherFrame) const;
    /**@}**/

    /** @name Frame Ancestry 
        These methods enable algorithms to employ Frames efficiently.
    */
    /**@{**/
    /** 
    A base Frame is the furthest Frame in a Frame's ancestry (e.g. itself,
    it's parent, grandparent, great-grandparent, etc... up the family tree)
    whose angular velocity is identical to this Frame. That is they represnt
    the same spatial entity. For example, anatomical frames may be used
    to identify points of intereset (muscle attachments) and joint connections
    on bodies in a convenient way, but they still represent he same Body.
    That body, in this case would be a base frame for any of the anatomical
    attached to the body including frames attached to other anatomical frames.

    @return baseFrame     The Frame that represents the base for this Frame.
    */
    const Frame& findBaseFrame() const;

    /**
    Find the equivalent Transform of this Frame (F) in its base (B) Frame.
    That is find X_BF, such that vecB = X_BF*vecF
    For a Frame that is itself a base, this returns the identity Transform.
    @return X_BF     The Transform of F in B
    */
    SimTK::Transform findTransformInBaseFrame() const;

    /**@}**/

private:
    /** @name Extension methods.
        Concrete Frame types must override these methods. */
    /**@{**/

    /** Calculate the transform related to this Frame with respect to ground.
    This method returns the transform X_GF, converting quantities expressed
    in this frame, F, to quantities expressed in the ground, G, frame.
    This is mathematically stated as:
        vec_G = X_GF*vec_F  */
    virtual const SimTK::Transform&
        calcGroundTransform(const SimTK::State& state) const = 0;

    /** Extend how concrete Frame determines its base Frame. */
    virtual const Frame& extendFindBaseFrame() const = 0;
    virtual SimTK::Transform extendFindTransformInBaseFrame() const = 0;
    /**@}**/

//=============================================================================
};  // END of class Frame
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // OPENSIM_FRAME_H_


