/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseRequest.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaioumy <abaioumy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 10:04:02 by abaioumy          #+#    #+#             */
/*   Updated: 2023/05/25 10:06:03 by abaioumy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>

class ParseRequest
{
    public:
        ParseRequest( void );
        ~ParseRequest( void );
        const char *getFileType( const char *path ) const;
        
    private:
};