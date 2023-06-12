/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   States.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:31:39 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/12 14:27:27 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ClientManager.hpp"


enum states
{
    READ_REQUEST,
    NO_STATE,
    WRITE_RESPONSE
};
// class States
// {
//     public:
//         virtual void handleState( ClientManager *client ) = 0;                
//         void    reset( ClientManager *client );
// };