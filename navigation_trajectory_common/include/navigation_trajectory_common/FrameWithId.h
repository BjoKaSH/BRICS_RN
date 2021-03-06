/******************************************************************************
 * Copyright (c) 2011
 * GPS GmbH
 *
 * Author:
 * Alexey Zakharov
 *
 *
 * This software is published under a dual-license: GNU Lesser General Public
 * License LGPL 2.1 and BSD license. The dual-license implies that users of this
 * code may choose which terms they prefer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of GPS GmbH nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version or the BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License LGPL and the BSD license for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL and BSD license along with this program.
 *
 ******************************************************************************/

#ifndef FRAMEWITHID_H
#define	FRAMEWITHID_H

#include <string>

namespace KDL {
    class Frame;
}

/**
 * @brief FrameWithId is an aggregation of KDL::Frame class with a std::string as frame id. 
 * The embedded KDL::Frame will be always heap-allocated.
 */

class FrameWithId {
public:
    /**
     * @brief String identifier of the frame
     */
    std::string id;

public:

    /**
     * @brief Constructor
     * 
     * The embedded KDL::Frame member will be initialized by its default constructor.
     *  
     * @param[in] id - string identifier of the frame
     */
    FrameWithId(const std::string& id);
    
    /**
     * @brief Constructor
     * 
     * The frame's id is set to be the empty string.
     * 
     * @param[in] frame - initial 6D pose of the frame
     */
    FrameWithId(const KDL::Frame& frame);
    
    /**
     * @brief Constructor
     * @param[in] frame - initial 6D pose of the frame
     * @param[in] id - string identifier of the frame
     */
    FrameWithId(const KDL::Frame& frame, const std::string& id);

    /**
     * @brief Default constructor
     * 
     * The embedded KDL::Frame member will be initialized by its default constructor.
     * The frame's id is set to be the empty string.
     */    
    FrameWithId();

    /**
     * @brief Copy constructor
     * @param[in] arg - reference to the original object
     */
    FrameWithId(const FrameWithId& orig);
    
    /**
     * @brief Operation assignment
     * @param[in] orig - reference to the original object
     */
    const FrameWithId& operator=(const FrameWithId& orig);
    
    /**
     * @brief Sets new KDL::Frame
     * 
     * The supplied KDL::Frame @p frame is copied and can be delete or modified 
     * without affecting the FrameWithId instance.
     * The @c id member is not modified.
     * 
     */
    void setFrame(const KDL::Frame& frame);
        
    /**
     * @brief Returns an underlying KDL::Frame as a const reference.
     * 
     * Modifications on the FrameWithId, except assignment or setFrame(), will be 
     * reflected by the returned reference.
     * 
     * Assigning or setting a new frame invalidates all references obtained from 
     * const KDL::Frame& getFrame() const.
     * 
     */
    const KDL::Frame& getFrame() const;
    
    /**
     * @brief Returns an underlying KDL::Frame as a reference
     * 
     * Modifications on the FrameWithId, except assignment or setFrame(), will be 
     * reflected by the returned reference.
     * 
     * Assigning or setting a new frame invalidates all references obtained from 
     * KDL::Frame& getFrame().
     * 
     */
    KDL::Frame& getFrame();

    virtual ~FrameWithId();
    
private:
    /**
     * @brief Aggregated, heap-allocated KDL::Frame.
     */
    KDL::Frame* frame; 
 
};


#endif	/* FRAMEWITHID_H */

