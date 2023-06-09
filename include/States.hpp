/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   States.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:31:39 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/09 12:35:35 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientInfo.hpp"

class States
{
    public:
        virtual void handleState( ClientInfo *client ) = 0;                
        void    reset( ClientInfo *client );
};