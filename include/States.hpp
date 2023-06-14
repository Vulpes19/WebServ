/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   States.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 12:31:39 by abaioumy          #+#    #+#             */
/*   Updated: 2023/06/14 11:28:52 by abaioumy         ###   ########.fr       */
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

enum ResponseStates
{
    RESET,
    READING,
    ERROR_FOUND,
    READY_TO_WRITE
};
// class States
// {
//     public:
//         virtual void handleState( ClientManager *client ) = 0;                
//         void    reset( ClientManager *client );
// };